#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/ObjectLocker.hpp>
#include <redstrain/platform/ThreadingError.hpp>
#include <redstrain/platform/utils.hpp>

#include "Q6Engine.hpp"
#include "PollingProviderSource.hpp"
#include "tweaks.hpp"

using redengine::vfs::URI;
using redengine::util::Delete;
using redengine::text::String16;
using redengine::util::FileSize;
using redengine::platform::Thread;
using redengine::platform::TypedThread;
using redengine::platform::ObjectLocker;
using redengine::platform::ThreadingError;
using redengine::platform::currentTimeMillis;

namespace redengine {
namespace qu6ntum {

	// ======== PollThread ========

	PollingProviderSource::PollThread::PollThread(PollingProviderSource& providerSource)
			: providerSource(providerSource) {}

	PollingProviderSource::PollThread::PollThread(const PollThread& thread)
			: TypedThread<>(thread), providerSource(providerSource) {}

	void PollingProviderSource::PollThread::run() {
		uint64_t now = currentTimeMillis();
		uint64_t nextInterrupt = now + providerSource.interruptInterval;
		uint64_t nextPoll = now + providerSource.pollInterval;
		for(;;) {
			uint64_t sleepUntil = nextInterrupt < nextPoll ? nextInterrupt : nextPoll;
			Thread::sleep(sleepUntil - now);
			if(providerSource.shouldStop)
				break;
			now = currentTimeMillis();
			if(now >= nextPoll) {
				providerSource.beginPoll();
				providerSource.pollProviders();
				providerSource.endPoll();
				nextPoll = now + providerSource.pollInterval;
			}
			nextInterrupt = now + providerSource.interruptInterval;
		}
	}

	// ======== KnownProvider ========

	PollingProviderSource::KnownProvider::KnownProvider(const String16& name, time_t lastModified, FileSize lastSize)
			: name(name), lastModified(lastModified), lastSize(lastSize),
			presentInEngine(false), updatePending(false) {}

	PollingProviderSource::KnownProvider::KnownProvider(const KnownProvider& provider)
			: name(provider.name), lastModified(provider.lastModified), lastSize(provider.lastSize),
			presentInEngine(provider.presentInEngine), updatePending(provider.updatePending) {}

	// ======== PollingProviderSource ========

	PollingProviderSource::PollingProviderSource()
			: pollInterval(static_cast<uint64_t>(REDSTRAIN_QU6NTUM_POLLINGPROVIDERSOURCE_DEFAULT_POLL_INTERVAL)),
			interruptInterval(static_cast<uint64_t>(
				REDSTRAIN_QU6NTUM_POLLINGPROVIDERSOURCE_DEFAULT_INTERRUPT_INTERVAL)),
			pollThread(NULL), shouldStop(false), engine(NULL) {}

	PollingProviderSource::PollingProviderSource(const PollingProviderSource& source) : ProviderSource(source),
			pollInterval(source.pollInterval), interruptInterval(source.interruptInterval), pollThread(NULL),
			shouldStop(false), engine(source.engine) {}

	PollingProviderSource::~PollingProviderSource() {
		if(pollThread) {
			shouldStop = true;
			try {
				pollThread->join();
			}
			catch(const ThreadingError&) {}
			delete pollThread;
		}
		KnownMap::const_iterator kmbegin(knownMap.begin()), kmend(knownMap.end());
		for(; kmbegin != kmend; ++kmbegin)
			delete kmbegin->second;
	}

	void PollingProviderSource::setPollInterval(uint64_t interval) {
		pollInterval = interval ? interval
				: static_cast<uint64_t>(REDSTRAIN_QU6NTUM_POLLINGPROVIDERSOURCE_DEFAULT_POLL_INTERVAL);
	}

	void PollingProviderSource::setInterruptInterval(uint64_t interval) {
		interruptInterval = interval ? interval
				: static_cast<uint64_t>(REDSTRAIN_QU6NTUM_POLLINGPROVIDERSOURCE_DEFAULT_INTERRUPT_INTERVAL);
	}

	void PollingProviderSource::startProviderSource(Q6Engine& engine) {
		ObjectLocker<PollingProviderSource> lock(this);
		if(pollThread) {
			lock.release();
			return;
		}
		this->engine = &engine;
		shouldStop = false;
		pollThread = new PollThread(*this);
		pollThread->start();
		lock.release();
	}

	void PollingProviderSource::stopProviderSource(Q6Engine&) {
		ObjectLocker<PollingProviderSource> lock(this);
		if(!pollThread) {
			lock.release();
			return;
		}
		shouldStop = true;
		pollThread->join();
		delete pollThread;
		pollThread = NULL;
		lock.release();
	}

	void PollingProviderSource::beginPoll() {
		knownSet.clear();
	}

	void PollingProviderSource::polledProvider(const String16& name, time_t lastModified, FileSize size) {
		KnownMap::const_iterator it = knownMap.find(name);
		if(it == knownMap.end()) {
			// new provider
			Delete<KnownProvider> newProvider(new KnownProvider(name, lastModified, size));
			knownMap[name] = *newProvider;
			KnownProvider* bare = newProvider.set();
			knownSet.insert(bare);
		}
		else {
			// known provider
			KnownProvider& present = *it->second;
			if(lastModified != present.lastModified || size != present.lastSize) {
				// changed: defer flush until next poll
				present.lastModified = lastModified;
				present.lastSize = size;
				if(present.presentInEngine)
					present.updatePending = true;
			}
			else {
				// nothing changed, flush if neccessary
				if(!present.presentInEngine) {
					present.presentInEngine = true;
					Delete<URI> uri(getProviderURIByName(present.name));
					engine->notifyProviderChange(**uri, Q6Engine::FSC_CREATED);
				}
				else if(present.updatePending) {
					present.updatePending = false;
					Delete<URI> uri(getProviderURIByName(present.name));
					engine->notifyProviderChange(**uri, Q6Engine::FSC_REMOVED);
				}
			}
			knownSet.insert(&present);
		}
	}

	void PollingProviderSource::endPoll() {
		KnownSet dead;
		KnownMap::iterator mbegin(knownMap.begin()), mend(knownMap.end());
		for(; mbegin != mend; ++mbegin) {
			if(knownSet.find(mbegin->second) == knownSet.end())
				dead.insert(mbegin->second);
		}
		knownSet.clear();
		KnownSet::const_iterator sbegin(dead.begin()), send(dead.end());
		for(; sbegin != send; ++sbegin) {
			knownMap.erase((*sbegin)->name);
			Delete<KnownProvider> known(*sbegin);
			Delete<URI> uri(getProviderURIByName(known->name));
			engine->notifyProviderChange(**uri, Q6Engine::FSC_REMOVED);
		}
	}

}}

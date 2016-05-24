#include <redstrain/util/Delete.hpp>
#include <redstrain/vfs/FileURI.hpp>
#include <redstrain/vfs/HostVFS.hpp>
#include <redstrain/vfs/RelativeURI.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Library.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/platform/MutexLocker.hpp>
#include <redstrain/platform/ObjectLocker.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/text/DefaultIntegerFormatter.hpp>
#include <redstrain/text/DefaultFormattingRendition.hpp>
#include <redstrain/text/DefaultFormattingOptionStringEmitter.hpp>

#include "Q6Engine.hpp"
#include "ProviderSource.hpp"
#include "EngineListener.hpp"

using std::string;
using redengine::vfs::URI;
using redengine::vfs::FileURI;
using redengine::vfs::HostVFS;
using redengine::error::Error;
using redengine::util::Delete;
using redengine::text::String16;
using redengine::io::InputStream;
using redengine::vfs::RelativeURI;
using redengine::io::StreamCloser;
using redengine::util::StringUtils;
using redengine::platform::Library;
using redengine::platform::Pathname;
using redengine::vfs::URIAcquisition;
using redengine::io::FileOutputStream;
using redengine::platform::Filesystem;
using redengine::platform::TypedThread;
using redengine::platform::MutexLocker;
using redengine::platform::ObjectLocker;
using redengine::error::ProgrammingError;
using redengine::text::FormattingOptions;
using redengine::text::DefaultIntegerFormatter;
using redengine::text::DefaultFormattingRendition;
using redengine::text::DefaultFormattingOptionStringEmitter;

namespace redengine {
namespace qu6ntum {

	// ======== Init ========

	Q6Engine::Init::Init(Q6Engine& engine, QueuedAction initialAction)
			: engine(engine), initialAction(initialAction) {}

	Q6Engine::Init::Init(const Init& init)
			: TypedThread<>(init), engine(init.engine), initialAction(init.initialAction) {}

	Q6Engine::Init::~Init() {}

	void Q6Engine::Init::run() {
		QueuedAction nextAction = initialAction;
		for(;;) {
			EngineState nextState = nextAction == QA_START_UP ? ES_STARTING_UP : ES_SHUTTING_DOWN;
			{
				ObjectLocker<int> lock(&engine.engineStateLock);
				engine.engineState = nextState;
				lock.release();
			}
			if(nextAction == QA_START_UP)
				engine.startupImpl();
			else
				engine.shutdownImpl();
			{
				ObjectLocker<int> lock(&engine.engineStateLock);
				if(actionQueue.empty()) {
					engine.initThread = NULL;
					if(engine.shouldTerminate) {
						MutexLocker tclock(engine.terminateCondition.getMutex());
						engine.terminateCondition.broadcast();
						tclock.release();
					}
					delete this;
					lock.release();
					return;
				}
				nextAction = actionQueue.front();
				actionQueue.pop_front();
				lock.release();
			}
		}
	}

	void Q6Engine::Init::queueAction(QueuedAction action) {
		ObjectLocker<int> lock(&engine.engineStateLock);
		if(actionQueue.empty() || action != actionQueue.back())
			actionQueue.push_back(action);
		lock.release();
	}

	// ======== Q6Engine ========

	Q6Engine::Q6Engine() : engineState(ES_SHUT_DOWN), initThread(NULL), shouldTerminate(false),
			providerSourcesState(ES_SHUT_DOWN), nextProviderID(static_cast<ProviderInstanceID>(0u)) {
		setProviderCacheDirectory(string());
		instanceHome = new FileURI(HostVFS::instance.getWorkingDirectory());
	}

	Q6Engine::Q6Engine(const Q6Engine& engine)
			: engineState(engine.engineState), initThread(NULL), shouldTerminate(false),
			providerSourcesState(ES_SHUT_DOWN), providerCacheDirectory(engine.providerCacheDirectory),
			nextProviderID(engine.nextProviderID) {
		instanceHome = engine.instanceHome->clone();
	}

	Q6Engine::~Q6Engine() {
		delete instanceHome;
		if(initThread)
			delete initThread;
		ProviderSources::const_iterator psbegin(providerSources.begin()), psend(providerSources.end());
		for(; psbegin != psend; ++psbegin)
			delete *psbegin;
		EngineListeners::const_iterator elbegin(engineListeners.begin()), elend(engineListeners.end());
		for(; elbegin != elend; ++elbegin)
			delete *elbegin;
	}

	void Q6Engine::setInstanceHome(const URI& home) {
		Delete<URI> newHome(instanceHome->resolve(home));
		delete instanceHome;
		instanceHome = newHome.set();
	}

	void Q6Engine::setInstanceHome(const String16& home) {
		RelativeURI rel(home);
		setInstanceHome(rel);
	}

	void Q6Engine::setInstanceHome(const string& home) {
		RelativeURI rel(home);
		setInstanceHome(rel);
	}

	void Q6Engine::getEngineListeners(EngineListeners& listeners) {
		ObjectLocker<EngineListeners> lock(&engineListeners);
		listeners = engineListeners;
		lock.release();
	}

	bool Q6Engine::addEngineListener(EngineListener& listener) {
		ObjectLocker<EngineListeners> lock(&engineListeners);
		if(engineListeners.find(&listener) != engineListeners.end()) {
			lock.release();
			return false;
		}
		engineListeners.insert(&listener);
		lock.release();
		return true;
	}

	bool Q6Engine::removeEngineListener(EngineListener& listener) {
		ObjectLocker<EngineListeners> lock(&engineListeners);
		EngineListeners::iterator it = engineListeners.find(&listener);
		if(it == engineListeners.end()) {
			lock.release();
			return false;
		}
		engineListeners.erase(it);
		lock.release();
		return true;
	}

#define forAllEngineListeners \
	ObjectLocker<EngineListeners> lock(&engineListeners); \
	EngineListeners cache(engineListeners); \
	lock.release(); \
	EngineListeners::const_iterator begin(cache.begin()), end(cache.end()); \
	for(; begin != end; ++begin)

	void Q6Engine::fireStartingUp() {
		forAllEngineListeners
			(*begin)->startingUp(*this);
	}

	void Q6Engine::fireStartupComplete() {
		forAllEngineListeners
			(*begin)->startupComplete(*this);
	}

	void Q6Engine::fireShuttingDown() {
		forAllEngineListeners
			(*begin)->shuttingDown(*this);
	}

	void Q6Engine::fireShutdownComplete() {
		forAllEngineListeners
			(*begin)->shutdownComplete(*this);
	}

	void Q6Engine::fireStartingProviderSource(ProviderSource& source) {
		forAllEngineListeners
			(*begin)->startingProviderSource(*this, source);
	}

	void Q6Engine::fireStartingProviderSourceFailed(ProviderSource& source, const Error& error) {
		forAllEngineListeners
			(*begin)->startingProviderSourceFailed(*this, source, error);
	}

	void Q6Engine::fireStoppingProviderSource(ProviderSource& source) {
		forAllEngineListeners
			(*begin)->stoppingProviderSource(*this, source);
	}

	void Q6Engine::fireStoppingProviderSourceFailed(ProviderSource& source, const Error& error) {
		forAllEngineListeners
			(*begin)->stoppingProviderSourceFailed(*this, source, error);
	}

	void Q6Engine::startProviderSource(ProviderSource& source) {
		fireStartingProviderSource(source);
		try {
			source.startProviderSource(*this);
		}
		catch(const Error& error) {
			fireStartingProviderSourceFailed(source, error);
		}
	}

	void Q6Engine::stopProviderSource(ProviderSource& source) {
		fireStoppingProviderSource(source);
		try {
			source.stopProviderSource(*this);
		}
		catch(const Error& error) {
			fireStoppingProviderSourceFailed(source, error);
		}
	}

	void Q6Engine::clearEngineListeners() {
		ObjectLocker<EngineListeners> lock(&engineListeners);
		EngineListeners::const_iterator begin(engineListeners.begin()), end(engineListeners.end());
		for(; begin != end; ++begin)
			delete *begin;
		engineListeners.clear();
		lock.release();
	}

	void Q6Engine::setProviderCacheDirectory(const string& directory) {
		if(directory.empty())
			providerCacheDirectory = Filesystem::makeTempDirectory();
		else
			providerCacheDirectory = directory;
	}

	bool Q6Engine::startup() {
		ObjectLocker<int> lock(&engineStateLock);
		if(shouldTerminate) {
			lock.release();
			return false;
		}
		switch(engineState) {
			case ES_SHUT_DOWN:
				engineState = ES_STARTING_UP;
				initThread = new Init(*this, Init::QA_START_UP);
				initThread->start();
				lock.release();
				return true;
			case ES_RUNNING:
				lock.release();
				return false;
			case ES_STARTING_UP:
			case ES_SHUTTING_DOWN:
				initThread->queueAction(Init::QA_START_UP);
				lock.release();
				return true;
			default:
				lock.release();
				throw ProgrammingError("Unrecognized EngineState: "
						+ StringUtils::toString(static_cast<int>(engineState)));
		}
	}

	bool Q6Engine::shutdown() {
		ObjectLocker<int> lock(&engineStateLock);
		switch(engineState) {
			case ES_SHUT_DOWN:
				lock.release();
				return false;
			case ES_RUNNING:
				engineState = ES_SHUTTING_DOWN;
				initThread = new Init(*this, Init::QA_SHUT_DOWN);
				initThread->start();
				lock.release();
				return true;
			case ES_STARTING_UP:
			case ES_SHUTTING_DOWN:
				initThread->queueAction(Init::QA_SHUT_DOWN);
				lock.release();
				return true;
			default:
				lock.release();
				throw ProgrammingError("Unrecognized EngineState: "
						+ StringUtils::toString(static_cast<int>(engineState)));
		}
	}

	void Q6Engine::waitForTermination(bool requestShutdown) {
		ObjectLocker<int> lock(&engineStateLock);
		shouldTerminate = true;
		MutexLocker tclock(terminateCondition.getMutex());
		if(requestShutdown)
			shutdown();
		lock.release();
		terminateCondition.wait();
		tclock.release();
	}

	void Q6Engine::startupImpl() {
		fireStartingUp();
		ObjectLocker<ProviderSources> pslock(&providerSources);
		ProviderSources pscache(providerSources);
		pslock.release();
		ProviderSources::const_iterator psbegin(pscache.begin()), psend(pscache.end());
		for(; psbegin != psend; ++psbegin)
			startProviderSource(**psbegin);
		fireStartupComplete();
	}

	void Q6Engine::shutdownImpl() {
		fireShuttingDown();
		ObjectLocker<ProviderSources> pslock(&providerSources);
		ProviderSources pscache(providerSources);
		pslock.release();
		ProviderSources::const_iterator psbegin(pscache.begin()), psend(pscache.end());
		for(; psbegin != psend; ++psbegin)
			stopProviderSource(**psbegin);
		fireShutdownComplete();
	}

	void Q6Engine::getProviderSources(ProviderSources& providerSources) {
		ObjectLocker<ProviderSources> lock(&this->providerSources);
		providerSources = this->providerSources;
		lock.release();
	}

	bool Q6Engine::addProviderSource(ProviderSource& source) {
		ObjectLocker<ProviderSources> lock(&providerSources);
		if(providerSources.find(&source) != providerSources.end()) {
			lock.release();
			return false;
		}
		providerSources.insert(&source);
		switch(providerSourcesState) {
			case ES_SHUT_DOWN:
			case ES_SHUTTING_DOWN:
				break;
			case ES_STARTING_UP:
			case ES_RUNNING:
				startProviderSource(source);
				break;
			default:
				lock.release();
				throw ProgrammingError("Unrecognized EngineState: "
						+ StringUtils::toString(static_cast<int>(engineState)));
		}
		lock.release();
		return true;
	}

	bool Q6Engine::removeProviderSource(ProviderSource& source) {
		ObjectLocker<ProviderSources> lock(&providerSources);
		ProviderSources::iterator it = providerSources.find(&source);
		if(it == providerSources.end()) {
			lock.release();
			return false;
		}
		providerSources.erase(it);
		if(providerSourcesState == ES_RUNNING)
			stopProviderSource(source);
		lock.release();
		return true;
	}

	void Q6Engine::clearProviderSources() {
		ObjectLocker<ProviderSources> lock(&providerSources);
		ProviderSources::const_iterator begin(providerSources.begin()), end(providerSources.end());
		for(; begin != end; ++begin) {
			if(providerSourcesState == ES_RUNNING)
				stopProviderSource(**begin);
			delete *begin;
		}
		providerSources.clear();
		lock.release();
	}

	static FormattingOptions<char>
			providerInstanceIDFormattingOptions(DefaultFormattingOptionStringEmitter<char>::instance,
			static_cast<uint8_t>(16u), static_cast<int32_t>(16u));

	ProviderInstanceID Q6Engine::cacheProviderCodebase(const URI& uri, string& dllPath) {
		Filesystem::mkdirRecursive(providerCacheDirectory);
		ObjectLocker<int> idLock(&providerIDLock);
		ProviderInstanceID id = nextProviderID++;
		idLock.release();
		dllPath = Pathname::join(providerCacheDirectory,
				DefaultIntegerFormatter<char>::formatInteger<ProviderInstanceID,
				DefaultFormattingRendition<char> >(id,
				providerInstanceIDFormattingOptions) + Library::FILENAME_SUFFIX);
		Delete<URIAcquisition<InputStream<char> > > acquisition(uri.acquireResource());
		acquisition->requestResource();
		InputStream<char>* in = acquisition->getAcquiredResource();
		StreamCloser inCloser(in);
		FileOutputStream out(dllPath);
		StreamCloser outCloser(out);
		in->copyTo(out);
		outCloser.close();
		inCloser.close();
		acquisition->releaseAcquiredResource();
		return id;
	}

	void Q6Engine::notifyProviderChange(const URI&, FileStateChange) {
		//TODO
	}

}}

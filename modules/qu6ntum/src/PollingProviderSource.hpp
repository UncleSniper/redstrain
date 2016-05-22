#ifndef REDSTRAIN_MOD_QU6NTUM_POLLINGPROVIDERSOURCE_HPP
#define REDSTRAIN_MOD_QU6NTUM_POLLINGPROVIDERSOURCE_HPP

#include <map>
#include <set>
#include <ctime>
#include <redstrain/vfs/URI.hpp>
#include <redstrain/platform/TypedThread.hpp>
#include <redstrain/text/types.hpp>
#include <redstrain/util/types.hpp>

#include "ProviderSource.hpp"

namespace redengine {
namespace qu6ntum {

	class REDSTRAIN_QU6NTUM_API PollingProviderSource : public ProviderSource {

	  private:
		class REDSTRAIN_QU6NTUM_API PollThread : public platform::TypedThread<> {

		  private:
			PollingProviderSource& providerSource;

		  protected:
			virtual void run();

		  public:
			PollThread(PollingProviderSource&);
			PollThread(const PollThread&);

		};

		struct REDSTRAIN_QU6NTUM_API KnownProvider {

			const text::String16 name;
			time_t lastModified;
			util::FileSize lastSize;
			bool presentInEngine, updatePending;

			KnownProvider(const text::String16&, time_t, util::FileSize);
			KnownProvider(const KnownProvider&);

		};

	  private:
		typedef std::map<text::String16, KnownProvider*> KnownMap;
		typedef std::set<KnownProvider*> KnownSet;

	  private:
		volatile uint64_t pollInterval;
		volatile uint64_t interruptInterval;
		PollThread* pollThread;
		volatile bool shouldStop;
		Q6Engine* engine;
		KnownMap knownMap;
		KnownSet knownSet;

	  private:
		void beginPoll();
		void endPoll();

	  protected:
		void polledProvider(const text::String16&, time_t, util::FileSize);

		virtual void pollProviders() = 0;
		virtual vfs::URI* getProviderURIByName(const text::String16&) = 0;

	  public:
		PollingProviderSource();
		PollingProviderSource(const PollingProviderSource&);
		virtual ~PollingProviderSource();

		inline uint64_t getPollInterval() const {
			return pollInterval;
		}

		void setPollInterval(uint64_t);

		inline uint64_t getInterruptInterval() const {
			return interruptInterval;
		}

		void setInterruptInterval(uint64_t);

		virtual void startProviderSource(Q6Engine&);
		virtual void stopProviderSource(Q6Engine&);

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_POLLINGPROVIDERSOURCE_HPP */

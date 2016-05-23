#ifndef REDSTRAIN_MOD_QU6NTUM_Q6ENGINE_HPP
#define REDSTRAIN_MOD_QU6NTUM_Q6ENGINE_HPP

#include <set>
#include <deque>
#include <redstrain/vfs/URI.hpp>
#include <redstrain/error/Error.hpp>
#include <redstrain/platform/TypedThread.hpp>
#include <redstrain/platform/ConditionVariable.hpp>

#include "api.hpp"

namespace redengine {
namespace qu6ntum {

	class ProviderSource;
	class ManifestSource;
	class EngineListener;

	class REDSTRAIN_QU6NTUM_API Q6Engine {

	  public:
		enum EngineState {
			ES_SHUT_DOWN,
			ES_STARTING_UP,
			ES_RUNNING,
			ES_SHUTTING_DOWN
		};

		enum FileStateChange {
			FSC_CREATED,
			FSC_MODIFIED,
			FSC_REMOVED
		};

	  private:
		class REDSTRAIN_QU6NTUM_API Init : public platform::TypedThread<> {

		  public:
			enum QueuedAction {
				QA_START_UP,
				QA_SHUT_DOWN
			};

		  private:
			typedef std::deque<QueuedAction> ActionQueue;

		  private:
			Q6Engine& engine;
			QueuedAction initialAction;
			ActionQueue actionQueue;

		  protected:
			virtual void run();

		  public:
			Init(Q6Engine&, QueuedAction);
			Init(const Init&);
			virtual ~Init();

			void queueAction(QueuedAction);

		};

	  public:
		typedef std::set<ProviderSource*> ProviderSources;
		typedef std::set<EngineListener*> EngineListeners;

	  private:
		vfs::URI* instanceHome;
		volatile EngineState engineState;
		int engineStateLock;
		ProviderSources providerSources;
		Init *volatile initThread;
		volatile bool shouldTerminate;
		platform::ConditionVariable terminateCondition;
		volatile EngineState providerSourcesState;
		EngineListeners engineListeners;

	  private:
		void startupImpl();
		void shutdownImpl();

		void fireStartingUp();
		void fireStartupComplete();
		void fireShuttingDown();
		void fireShutdownComplete();
		void fireStartingProviderSource(ProviderSource&);
		void fireStartingProviderSourceFailed(ProviderSource&, const error::Error&);
		void fireStoppingProviderSource(ProviderSource&);
		void fireStoppingProviderSourceFailed(ProviderSource&, const error::Error&);

		void startProviderSource(ProviderSource&);
		void stopProviderSource(ProviderSource&);

	  public:
		Q6Engine();
		Q6Engine(const Q6Engine&);
		~Q6Engine();

		inline EngineState getEngineState() const {
			return engineState;
		}

		inline const vfs::URI& getInstanceHome() const {
			return *instanceHome;
		}

		void setInstanceHome(const vfs::URI&);
		void setInstanceHome(const text::String16&);
		void setInstanceHome(const std::string&);

		void getEngineListeners(EngineListeners&);
		bool addEngineListener(EngineListener&);
		bool removeEngineListener(EngineListener&);
		void clearEngineListeners();

		bool startup();
		bool shutdown();
		void waitForTermination(bool);

		void getProviderSources(ProviderSources&);
		bool addProviderSource(ProviderSource&);
		bool removeProviderSource(ProviderSource&);
		void clearProviderSources();

		void notifyProviderChange(const vfs::URI&, FileStateChange);

	};

}}

#endif /* REDSTRAIN_MOD_QU6NTUM_Q6ENGINE_HPP */

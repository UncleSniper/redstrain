#ifndef REDSTRAIN_MOD_PARALLEL_ABSTRACTTHREADPOOL_HPP
#define REDSTRAIN_MOD_PARALLEL_ABSTRACTTHREADPOOL_HPP

#include <set>
#include <map>
#include <list>
#include <stack>
#include <redstrain/platform/Mutex.hpp>
#include <redstrain/platform/TypedThread.hpp>
#include <redstrain/platform/ConditionVariable.hpp>

#include "ThreadPool.hpp"
#include "TaskCookie.hpp"
#include "PooledThread.hpp"

namespace redengine {
namespace parallel {

	class REDSTRAIN_PARALLEL_API AbstractThreadPool : public ThreadPool {

	  protected:
		enum REDSTRAIN_PARALLEL_API ThreadState {
			TS_AVAILABLE,
			TS_BUSY,
			TS_FULL
		};

		enum REDSTRAIN_PARALLEL_API ThreadGoal {
			TG_IDLE,
			TG_BEGIN_TASK,
			TG_TERMINATE
		};

		class REDSTRAIN_PARALLEL_API AbstractPooledThread : public PooledThread, public platform::TypedThread<> {

		  protected:
			typedef std::map<UntypedTask*, unsigned> TaskWaits;
			typedef TaskWaits::iterator TaskWaitIterator;
			typedef std::set<UntypedTask*> CompletedTasks;
			typedef CompletedTasks::iterator CompletedTaskIterator;
			typedef std::set<PooledThread*> ThreadSet;
			typedef std::map<UntypedTask*, ThreadSet> CompletionListeners;
			typedef CompletionListeners::iterator CompletionListenerIterator;

		  protected:
			AbstractThreadPool& pool;
			platform::ConditionVariable waitCondition;
			volatile ThreadState state;
			volatile ThreadGoal goal;
			UntypedTask *volatile slatedTask;
			volatile unsigned taskDepth;
			UntypedTask *volatile finishedTask;
			TaskWaits taskWaits;
			CompletedTasks completedTasks;
			CompletionListeners completionListeners;

		  private:
			void pumpTasks(UntypedTask*);
			void taskFinished(UntypedTask&);

		  protected:
			AbstractPooledThread(const AbstractPooledThread&);

			virtual void run();

		  public:
			AbstractPooledThread(AbstractThreadPool&, ThreadState);
			virtual ~AbstractPooledThread();

			inline unsigned getTaskDepth() const {
				return taskDepth;
			}

			bool submitTask(UntypedTask&, bool);
			void pleaseTerminate();
			bool tryTerminate();

			virtual bool isCallingThread();
			virtual void waitForTaskCompletion(UntypedTask&);
			virtual void forcingTask(UntypedTask&, bool);
			virtual bool requestNotifyOnTaskCompletion(PooledThread&, UntypedTask&);
			virtual void notifyTaskCompleted(UntypedTask&);

		};

		class REDSTRAIN_PARALLEL_API AbstractTaskCookie : public TaskCookie {

		  public:
			typedef std::list<AbstractTaskCookie*>::iterator QueueCookie;

		  private:
			AbstractThreadPool& pool;
			UntypedTask& task;
			QueueCookie queueCookie;

		  protected:
			AbstractTaskCookie(const AbstractTaskCookie&);

		  public:
			AbstractTaskCookie(AbstractThreadPool&, UntypedTask&);
			virtual ~AbstractTaskCookie();

			inline UntypedTask& getTask() {
				return task;
			}

			inline const UntypedTask& getTask() const {
				return task;
			}

			inline const QueueCookie& getQueueCookie() const {
				return queueCookie;
			}

			void setQueueCookie(const QueueCookie&);

			virtual void removeFromQueue();

		};

	  protected:
		typedef std::set<AbstractPooledThread*> Threads;
		typedef Threads::const_iterator ThreadIterator;
		typedef std::list<AbstractTaskCookie*> TaskQueue;
		typedef TaskQueue::const_iterator TaskQueueIterator;
		typedef std::map<platform::Thread::Handle, AbstractPooledThread*> ThreadByID;
		typedef ThreadByID::iterator ThreadByIDIterator;

	  protected:
		platform::Mutex poolMutex;
		Threads availableThreads, busyThreads, fullThreads;
		TaskQueue taskQueue;
		ThreadByID threadByID;

	  protected:
		AbstractThreadPool(const AbstractThreadPool&);

		AbstractTaskCookie* queueTask(UntypedTask&);

		virtual void goingIdle(AbstractPooledThread&) = 0;
		virtual void goingBusy(AbstractPooledThread&) = 0;
		virtual void threadFull(AbstractPooledThread&) = 0;
		virtual void leavingForce(AbstractPooledThread&) = 0;
		virtual AbstractPooledThread* expedite(UntypedTask&) = 0;

	  public:
		AbstractThreadPool();
		virtual ~AbstractThreadPool();

		virtual void force(UntypedTask&);

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_ABSTRACTTHREADPOOL_HPP */

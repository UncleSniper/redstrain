#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/MutexLocker.hpp>
#include <redstrain/platform/DeadlockError.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "UntypedTask.hpp"
#include "AbstractThreadPool.hpp"

using redengine::util::Delete;
using redengine::platform::Thread;
using redengine::util::StringUtils;
using redengine::platform::MutexLocker;
using redengine::platform::DeadlockError;
using redengine::error::ProgrammingError;

namespace redengine {
namespace parallel {

	// ======== AbstractPooledThread ========

	AbstractThreadPool::AbstractPooledThread::AbstractPooledThread(AbstractThreadPool& pool, ThreadState state)
			: pool(pool), state(state), goal(TG_IDLE), slatedTask(NULL), taskDepth(0u), finishedTask(NULL) {}

	AbstractThreadPool::AbstractPooledThread::AbstractPooledThread(const AbstractPooledThread& thread)
			: PooledThread(thread), TypedThread<>(thread), pool(thread.pool), state(TS_AVAILABLE), goal(TG_IDLE),
			slatedTask(NULL), taskDepth(0u), finishedTask(NULL) {}

	AbstractThreadPool::AbstractPooledThread::~AbstractPooledThread() {
		if(slatedTask)
			delete slatedTask;
	}

	void AbstractThreadPool::AbstractPooledThread::run() {
		pumpTasks(NULL);
	}

	void AbstractThreadPool::AbstractPooledThread::pumpTasks(UntypedTask* waitingFor) {
		MutexLocker lock(waitCondition.getMutex());
		for(;;) {
			switch(goal) {
				case TG_IDLE:
					if(waitingFor) {
						CompletedTaskIterator cti = completedTasks.find(waitingFor);
						if(cti != completedTasks.end()) {
							completedTasks.erase(cti);
							pool.leavingForce(*this);
							lock.release();
							return;
						}
					}
					pool.goingIdle(*this);
					waitCondition.wait();
					break;
				case TG_BEGIN_TASK:
					{
						UntypedTask* task = slatedTask;
						slatedTask = NULL;
						goal = TG_IDLE;
						state = TS_BUSY;
						pool.goingBusy(*this);
						++taskDepth;
						lock.release();
						task->executeInCurrentThread();
						lock.lock(waitCondition.getMutex());
						--taskDepth;
						state = TS_AVAILABLE;
						taskFinished(*task);
					}
					break;
				case TG_TERMINATE:
					return;
				default:
					throw ProgrammingError("Unrecognized ThreadGoal in "
							"AbstractThreadPool::AbstractPooledThread::run(): "
							+ StringUtils::toString(static_cast<int>(goal)));
			}
		}
	}

	void AbstractThreadPool::AbstractPooledThread::taskFinished(UntypedTask& task) {
		CompletionListenerIterator cli = completionListeners.find(&task);
		if(cli != completionListeners.end()) {
			const ThreadSet& listeners = cli->second;
			ThreadSet::const_iterator clbegin(listeners.begin()), clend(listeners.end());
			for(; clbegin != clend; ++clbegin)
				(*clbegin)->notifyTaskCompleted(task);
			completionListeners.erase(cli);
		}
		TaskWaitIterator wait = taskWaits.find(&task);
		if(wait == taskWaits.end()) {
			waitCondition.broadcast();
			return;
		}
		finishedTask = &task;
		while(wait->second)
			waitCondition.broadcast();
		finishedTask = NULL;
		taskWaits.erase(wait);
	}

	bool AbstractThreadPool::AbstractPooledThread::submitTask(UntypedTask& task, bool wait) {
		MutexLocker lock(waitCondition.getMutex());
		switch(state) {
			case TS_AVAILABLE:
				if(taskDepth >= pool.getMaxNestingLevel()) {
					state = TS_FULL;
					pool.threadFull(*this);
					lock.release();
					return false;
				}
				else if(wait && getHandle() == Thread::selfHandle()) {
					goal = TG_IDLE;
					state = TS_BUSY;
					pool.goingBusy(*this);
					++taskDepth;
					lock.release();
					task.executeInCurrentThread();
					lock.lock(waitCondition.getMutex());
					--taskDepth;
					state = TS_AVAILABLE;
					taskFinished(task);
				}
				else {
					slatedTask = &task;
					goal = TG_BEGIN_TASK;
					if(wait) {
						TaskWaitIterator twit = taskWaits.find(&task);
						if(twit == taskWaits.end())
							taskWaits[&task] = 1u;
						else
							++twit->second;
					}
					waitCondition.broadcast();
					if(wait) {
						for(;;) {
							waitCondition.wait();
							if(finishedTask == &task) {
								--taskWaits[&task];
								break;
							}
						}
					}
				}
				break;
			case TS_BUSY:
			case TS_FULL:
				lock.release();
				return false;
			default:
				throw ProgrammingError("Unrecognized ThreadGoal in "
						"AbstractThreadPool::AbstractPooledThread::submitTask(): "
						+ StringUtils::toString(static_cast<int>(goal)));
		}
		lock.release();
		return true;
	}

	void AbstractThreadPool::AbstractPooledThread::pleaseTerminate() {
		MutexLocker lock(waitCondition.getMutex());
		goal = TG_TERMINATE;
		waitCondition.broadcast();
		lock.release();
	}

	bool AbstractThreadPool::AbstractPooledThread::tryTerminate() {
		MutexLocker lock(waitCondition.getMutex());
		if(taskDepth || slatedTask) {
			lock.release();
			return false;
		}
		goal = TG_TERMINATE;
		waitCondition.broadcast();
		lock.release();
		return true;
	}

	bool AbstractThreadPool::AbstractPooledThread::isCallingThread() {
		return getHandle() == Thread::selfHandle();
	}

	void AbstractThreadPool::AbstractPooledThread::waitForTaskCompletion(UntypedTask& task) {
		MutexLocker lock(waitCondition.getMutex());
		if(task.isTaskFinished()) {
			lock.release();
			return;
		}
		TaskWaitIterator wait = taskWaits.find(&task);
		if(wait == taskWaits.end())
			taskWaits[&task] = 1u;
		else
			++wait->second;
		for(;;) {
			waitCondition.wait();
			if(finishedTask == &task) {
				--taskWaits[&task];
				break;
			}
		}
		lock.release();
	}

	void AbstractThreadPool::AbstractPooledThread::forcingTask(UntypedTask& task, bool pristine) {
		MutexLocker lock(waitCondition.getMutex());
		PooledThread* otherThread;
		if(pristine) {
			otherThread = pool.expedite(task);
			if(!otherThread || task.isTaskFinished()) {
				lock.release();
				return;
			}
		}
		else {
			otherThread = task.getExecutingThread();
			if(!otherThread || task.isTaskFinished()) {
				lock.release();
				return;
			}
			if(otherThread == this)
				throw DeadlockError();
		}
		bool willNotify = otherThread->requestNotifyOnTaskCompletion(*this, task);
		lock.release();
		if(willNotify)
			pumpTasks(&task);
	}

	bool AbstractThreadPool::AbstractPooledThread::requestNotifyOnTaskCompletion(PooledThread& otherThread,
			UntypedTask& task) {
		MutexLocker lock(waitCondition.getMutex());
		if(task.isTaskFinished()) {
			lock.release();
			return false;
		}
		ThreadSet& ts = completionListeners[&task];
		ts.insert(&otherThread);
		lock.release();
		return true;
	}

	void AbstractThreadPool::AbstractPooledThread::notifyTaskCompleted(UntypedTask& task) {
		MutexLocker lock(waitCondition.getMutex());
		completedTasks.insert(&task);
		waitCondition.broadcast();
		lock.release();
	}

	// ======== AbstractTaskCookie ========

	AbstractThreadPool::AbstractTaskCookie::AbstractTaskCookie(AbstractThreadPool& pool, UntypedTask& task)
			: pool(pool), task(task) {}

	AbstractThreadPool::AbstractTaskCookie::AbstractTaskCookie(const AbstractTaskCookie& cookie)
			: TaskCookie(cookie), pool(cookie.pool), task(cookie.task), queueCookie(cookie.queueCookie) {}

	AbstractThreadPool::AbstractTaskCookie::~AbstractTaskCookie() {}

	void AbstractThreadPool::AbstractTaskCookie::setQueueCookie(const QueueCookie& queueCookie) {
		this->queueCookie = queueCookie;
	}

	void AbstractThreadPool::AbstractTaskCookie::removeFromQueue() {
		MutexLocker lock(pool.poolMutex);
		pool.taskQueue.erase(queueCookie);
		lock.release();
	}

	// ======== AbstractThreadPool ========

	AbstractThreadPool::AbstractThreadPool() {}

	AbstractThreadPool::AbstractThreadPool(const AbstractThreadPool& pool) : ThreadPool(pool) {}

	AbstractThreadPool::~AbstractThreadPool() {
		ThreadIterator tbegin, tend;
#define forThreads(set) for(tbegin = set.begin(), tend = set.end(); tbegin != tend; ++tbegin)
#define requestTerminate(thread) \
	try { \
		(thread)->pleaseTerminate(); \
	} \
	catch(...) {}
#define waitTerminate(thread) \
	try { \
		(thread)->join(); \
		delete (thread); \
	} \
	catch(...) {}
		forThreads(availableThreads)
			requestTerminate(*tbegin);
		forThreads(busyThreads)
			requestTerminate(*tbegin);
		forThreads(fullThreads)
			requestTerminate(*tbegin);
		forThreads(availableThreads)
			waitTerminate(*tbegin);
		forThreads(busyThreads)
			waitTerminate(*tbegin);
		forThreads(fullThreads)
			waitTerminate(*tbegin);
#undef forThreads
	}

	AbstractThreadPool::AbstractTaskCookie* AbstractThreadPool::queueTask(UntypedTask& task) {
		Delete<AbstractTaskCookie> cookie(new AbstractTaskCookie(*this, task));
		AbstractTaskCookie::QueueCookie it(taskQueue.insert(taskQueue.end(), *cookie));
		cookie->setQueueCookie(it);
		task.setQueueCookie(*cookie);
		return cookie.set();
	}

	void AbstractThreadPool::force(UntypedTask& task) {
		MutexLocker lock(poolMutex);
		ThreadByIDIterator tbi = threadByID.find(Thread::selfHandle());
		AbstractPooledThread* caller = tbi == threadByID.end() ? NULL : tbi->second;
		TaskCookie* cookie = task.getQueueCookie();
		PooledThread* thread = task.getExecutingThread();
		if(cookie) {
			task.setQueueCookie(NULL);
			Delete<TaskCookie> deleteCookie(cookie);
			cookie->removeFromQueue();
			lock.release();
			if(caller)
				caller->forcingTask(task, true);
			else {
				AbstractPooledThread* t = expedite(task);
				if(t)
					t->waitForTaskCompletion(task);
			}
		}
		else if(thread) {
			lock.release();
			if(caller)
				caller->forcingTask(task, false);
			else
				thread->waitForTaskCompletion(task);
		}
		else
			lock.release();
	}

}}

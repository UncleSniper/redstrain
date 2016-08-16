#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/MutexLocker.hpp>
#include <redstrain/platform/utils.hpp>

#include "UntypedTask.hpp"
#include "FixedThreadPool.hpp"
#include "OutOfComputationalCapacityError.hpp"

using redengine::util::Delete;
using redengine::platform::Thread;
using redengine::platform::MutexLocker;
using redengine::platform::numberOfProcessors;

namespace redengine {
namespace parallel {

	FixedThreadPool::FixedThreadPool(unsigned desiredThreadCount)
			: desiredThreadCount(desiredThreadCount ? desiredThreadCount : numberOfProcessors() + 1u),
			actualThreadCount(0u) {
		balanceThreadCount();
	}

	FixedThreadPool::FixedThreadPool(const FixedThreadPool& pool)
			: AbstractThreadPool(pool), desiredThreadCount(pool.desiredThreadCount), actualThreadCount(0u) {
		balanceThreadCount();
	}

	void FixedThreadPool::setThreadCount(unsigned count) {
		desiredThreadCount = count ? count : numberOfProcessors() + 1u;
		balanceThreadCount();
	}

	void FixedThreadPool::balanceThreadCount() {
		MutexLocker lock(poolMutex);
		while(actualThreadCount < desiredThreadCount) {
			Delete<AbstractPooledThread> thread(new AbstractPooledThread(*this, TS_AVAILABLE));
			availableThreads.insert(*thread);
			++actualThreadCount;
			AbstractPooledThread* t = thread.set();
			t->start();
			threadByID[t->getHandle()] = t;
		}
		while(actualThreadCount > desiredThreadCount) {
			if(availableThreads.empty())
				break;
			Threads::iterator begin(availableThreads.begin()), end(availableThreads.end());
			for(; begin != end; ++begin) {
				if((*begin)->tryTerminate())
					break;
			}
			if(begin == end)
				break;
			AbstractPooledThread* t = *begin;
			availableThreads.erase(begin);
			Thread::Handle id = t->getHandle();
			threadByID.erase(id);
			--actualThreadCount;
			t->join();
			delete t;
		}
		lock.release();
	}

	void FixedThreadPool::goingIdle(AbstractPooledThread& thread) {
		MutexLocker lock(poolMutex);
		Threads::iterator it = busyThreads.find(&thread);
		if(it != busyThreads.end()) {
			busyThreads.erase(it);
			availableThreads.insert(&thread);
		}
		if(!taskQueue.empty()) {
			AbstractTaskCookie* cookie = taskQueue.front();
			if(thread.submitTask(cookie->getTask(), false)) {
				delete cookie;
				taskQueue.pop_front();
			}
		}
		lock.release();
	}

	void FixedThreadPool::goingBusy(AbstractPooledThread& thread) {
		MutexLocker lock(poolMutex);
		Threads::iterator it = availableThreads.find(&thread);
		if(it != busyThreads.end()) {
			availableThreads.erase(it);
			busyThreads.insert(&thread);
		}
		lock.release();
	}

	void FixedThreadPool::threadFull(AbstractPooledThread& thread) {
		MutexLocker lock(poolMutex);
		Threads::iterator it = availableThreads.find(&thread);
		if(it != availableThreads.end()) {
			availableThreads.erase(it);
			fullThreads.insert(&thread);
		}
		else {
			it = busyThreads.find(&thread);
			if(it != busyThreads.end()) {
				busyThreads.erase(it);
				fullThreads.insert(&thread);
			}
		}
		lock.release();
	}

	void FixedThreadPool::leavingForce(AbstractPooledThread& thread) {
		MutexLocker lock(poolMutex);
		Threads::iterator it = fullThreads.find(&thread);
		if(it != fullThreads.end()) {
			fullThreads.erase(it);
			busyThreads.insert(&thread);
		}
		lock.release();
	}

	AbstractThreadPool::AbstractPooledThread* FixedThreadPool::expedite(UntypedTask& task) {
		MutexLocker lock(poolMutex);
		ThreadIterator begin(availableThreads.begin()), end(availableThreads.end());
		for(; begin != end; ++begin) {
			if((*begin)->submitTask(task, false)) {
				AbstractPooledThread* t = *begin;
				lock.release();
				return t;
			}
		}
		ThreadByIDIterator tbi = threadByID.find(Thread::selfHandle());
		if(tbi != threadByID.end() && tbi->second->getTaskDepth() >= getMaxNestingLevel()) {
			lock.release();
			throw OutOfComputationalCapacityError();
		}
		lock.release();
		task.executeInCurrentThread();
		return NULL;
	}

	void FixedThreadPool::scheduleIntoThread(UntypedTask& task) {
		MutexLocker lock(poolMutex);
		ThreadIterator begin(availableThreads.begin()), end(availableThreads.end());
		for(; begin != end; ++begin) {
			if((*begin)->submitTask(task, false)) {
				lock.release();
				return;
			}
		}
		queueTask(task);
		lock.release();
	}

}}

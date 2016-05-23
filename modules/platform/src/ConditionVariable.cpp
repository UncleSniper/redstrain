#include <redstrain/util/IntegerBounds.hpp>

#include "IllegalConditionLockStateError.hpp"
#include "ConditionVariableOperationError.hpp"
#include "IllegalConditionVariableStateError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <time.h>
#include <errno.h>
#include <string.h>
#endif /* OS-specific includes */

using std::string;
using redengine::util::IntegerBounds;

namespace redengine {
namespace platform {

	ConditionVariable::ConditionVariable(const ConditionVariable&) {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	ConditionVariable::ConditionVariable() : destroyed(false) {
		int err = pthread_cond_init(&handle, NULL);
		if(err)
			throw ConditionVariableOperationError(ConditionVariableOperationError::CREATE, err);
	}

	ConditionVariable::~ConditionVariable() {
		if(!destroyed)
			pthread_cond_destroy(&handle);
	}

	bool ConditionVariable::wait(unsigned timeout) {
		if(destroyed)
			throw IllegalConditionVariableStateError(ConditionVariableOperationError::WAIT);
		int err;
		if(timeout == INFINITE_TIMEOUT)
			err = pthread_cond_wait(&handle, &const_cast<Mutex::Handle&>(mutex.getHandle()));
		else {
			struct timespec abstime;
			if(clock_gettime(CLOCK_REALTIME, &abstime))
				throw ConditionVariableOperationError(ConditionVariableOperationError::WAIT, errno);
			abstime.tv_sec += static_cast<time_t>(timeout / 1000u);
			abstime.tv_nsec += static_cast<long>(timeout % 1000u) * 1000000l;
			if(abstime.tv_nsec > 1000000000l) {
				++abstime.tv_sec;
				abstime.tv_nsec -= 1000000000l;
			}
			err = pthread_cond_timedwait(&handle, &const_cast<Mutex::Handle&>(mutex.getHandle()), &abstime);
			if(err == ETIMEDOUT)
				return false;
		}
		if(err) {
			if(err == EPERM)
				throw IllegalConditionLockStateError(ConditionVariableOperationError::WAIT);
			throw ConditionVariableOperationError(ConditionVariableOperationError::WAIT, err);
		}
		mutex._aWinnerIsMe();
		return true;
	}

	void ConditionVariable::signal() {
		if(destroyed)
			throw IllegalConditionVariableStateError(ConditionVariableOperationError::SIGNAL);
		mutex.lock();
		bool good = mutex.getLockDepth() > 1u && mutex.getOwner() == pthread_self();
		mutex.unlock();
		if(!good)
			throw IllegalConditionLockStateError(ConditionVariableOperationError::SIGNAL);
		int err = pthread_cond_signal(&handle);
		if(err)
			throw ConditionVariableOperationError(ConditionVariableOperationError::SIGNAL, err);
	}

	void ConditionVariable::broadcast() {
		if(destroyed)
			throw IllegalConditionVariableStateError(ConditionVariableOperationError::SIGNAL);
		mutex.lock();
		bool good = mutex.getLockDepth() > 1u && mutex.getOwner() == pthread_self();
		mutex.unlock();
		if(!good)
			throw IllegalConditionLockStateError(ConditionVariableOperationError::SIGNAL);
		int err = pthread_cond_broadcast(&handle);
		if(err)
			throw ConditionVariableOperationError(ConditionVariableOperationError::SIGNAL, err);
	}

	string ConditionVariable::getErrorMessage(ErrorCode error) {
		return strerror(error);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

#ifdef REDSTRAIN_PLATFORM_WINXP_COMPAT

	ConditionVariable::ConditionVariable() : destroyed(false) {
		handle.waitersCount = 0u;
		handle.wasBroadcast = false;
		handle.semaphore = CreateSemaphore(NULL, static_cast<LONG>(0l), IntegerBounds<LONG>::MAX, NULL);
		if(!handle.semaphore)
			throw ConditionVariableOperationError(ConditionVariableOperationError::CREATE, GetLastError());
		handle.waitersDone = CreateEvent(NULL, FALSE, FALSE, NULL);
		if(!handle.waitersDone) {
			CloseHandle(handle.semaphore);
			throw ConditionVariableOperationError(ConditionVariableOperationError::CREATE, GetLastError());
		}
		InitializeCriticalSection(&handle.waitersCountLock);
	}

	ConditionVariable::~ConditionVariable() {
		if(!destroyed) {
			CloseHandle(handle.semaphore);
			CloseHandle(handle.waitersDone);
			DeleteCriticalSection(&handle.waitersCountLock);
		}
	}

	bool ConditionVariable::wait(unsigned timeout) {
		if(destroyed)
			throw IllegalConditionVariableStateError(ConditionVariableOperationError::WAIT);
		EnterCriticalSection(&handle.waitersCountLock);
		++handle.waitersCount;
		LeaveCriticalSection(&handle.waitersCountLock);
	  waitAgain:
		switch(SignalObjectAndWait(mutex.getHandle(), handle.semaphore,
				timeout ? static_cast<DWORD>(timeout) : INFINITE,FALSE)) {
			case WAIT_ABANDONED:
				goto waitAgain;
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				EnterCriticalSection(&handle.waitersCountLock);
				--handle.waitersCount;
				LeaveCriticalSection(&handle.waitersCountLock);
				return false;
			default:
				EnterCriticalSection(&handle.waitersCountLock);
				--handle.waitersCount;
				LeaveCriticalSection(&handle.waitersCountLock);
				throw ConditionVariableOperationError(ConditionVariableOperationError::WAIT, GetLastError());
		}
		EnterCriticalSection(&handle.waitersCountLock);
		--handle.waitersCount;
		bool lastWaiter = handle.wasBroadcast && !handle.waitersCount;
		LeaveCriticalSection(&handle.waitersCountLock);
		if(lastWaiter) {
		  signalAgain:
			switch(SignalObjectAndWait(handle.waitersDone, mutex.getHandle(), INFINITE, FALSE)) {
				case WAIT_ABANDONED:
					goto signalAgain;
				case WAIT_OBJECT_0:
					break;
				default:
					throw ConditionVariableOperationError(ConditionVariableOperationError::WAIT,
							GetLastError());
			}
		}
		else {
		  acquireAgain:
			switch(WaitForSingleObject(mutex.getHandle(), INFINITE)) {
				case WAIT_ABANDONED:
					goto acquireAgain;
				case WAIT_OBJECT_0:
					break;
				default:
					throw ConditionVariableOperationError(ConditionVariableOperationError::WAIT,
							GetLastError());
			}
		}
		mutex._aWinnerIsMe();
		return true;
	}

	void ConditionVariable::signal() {
		if(destroyed)
			throw IllegalConditionVariableStateError(ConditionVariableOperationError::SIGNAL);
		mutex.lock();
		bool good = mutex.getLockDepth() > 1u && mutex.getOwner() == GetCurrentThreadId();
		mutex.unlock();
		if(!good)
			throw IllegalConditionLockStateError(ConditionVariableOperationError::SIGNAL);
		EnterCriticalSection(&handle.waitersCountLock);
		unsigned waiters = handle.waitersCount;
		LeaveCriticalSection(&handle.waitersCountLock);
		if(waiters) {
			if(!ReleaseSemaphore(handle.semaphore, static_cast<LONG>(1l), NULL))
				throw ConditionVariableOperationError(ConditionVariableOperationError::SIGNAL, GetLastError());
		}
	}

	void ConditionVariable::broadcast() {
		if(destroyed)
			throw IllegalConditionVariableStateError(ConditionVariableOperationError::SIGNAL);
		mutex.lock();
		bool good = mutex.getLockDepth() > 1u && mutex.getOwner() == GetCurrentThreadId();
		mutex.unlock();
		if(!good)
			throw IllegalConditionLockStateError(ConditionVariableOperationError::SIGNAL);
		EnterCriticalSection(&handle.waitersCountLock);
		if(handle.waitersCount) {
			handle.wasBroadcast = true;
			if(!ReleaseSemaphore(handle.semaphore, static_cast<LONG>(handle.waitersCount), NULL)) {
				LeaveCriticalSection(&handle.waitersCountLock);
				throw ConditionVariableOperationError(ConditionVariableOperationError::SIGNAL, GetLastError());
			}
			LeaveCriticalSection(&handle.waitersCountLock);
		  waitAgain:
			switch(WaitForSingleObject(handle.waitersDone, INFINITE)) {
				case WAIT_ABANDONED:
					goto waitAgain;
				case WAIT_OBJECT_0:
					break;
				default:
					throw ConditionVariableOperationError(ConditionVariableOperationError::SIGNAL,
							GetLastError());
			}
			handle.wasBroadcast = false;
		}
		else
			LeaveCriticalSection(&handle.waitersCountLock);
	}

#else /* Vista or above */

	ConditionVariable::ConditionVariable() : destroyed(false) {
		InitializeConditionVariable(&handle);
	}

	ConditionVariable::~ConditionVariable() {
		// MSDN doesn't say anything about how to destroy a
		// condition variable. I'm taking that as an indication
		// that such is not needed; after all, they do say
		// condition variables are pure userspace objects.
	}

	bool ConditionVariable::wait(unsigned timeout) {
		if(destroyed)
			throw IllegalConditionVariableStateError(ConditionVariableOperationError::WAIT);
		if(!SleepConditionVariableCS(&handle, const_cast<CRITICAL_SECTION*>(&mutex.getHandle()),
				timeout ? static_cast<DWORD>(timeout) : INFINITE)) {
			if(GetLastError() == ERROR_TIMEOUT)
				return false;
			throw ConditionVariableOperationError(ConditionVariableOperationError::WAIT, GetLastError());
		}
		mutex._aWinnerIsMe();
		return true;
	}

	void ConditionVariable::signal() {
		if(destroyed)
			throw IllegalConditionVariableStateError(ConditionVariableOperationError::SIGNAL);
		mutex.lock();
		bool good = mutex.getLockDepth() > 1u && mutex.getOwner() == GetCurrentThreadId();
		mutex.unlock();
		if(!good)
			throw IllegalConditionLockStateError(ConditionVariableOperationError::SIGNAL);
		WakeConditionVariable(&handle);
	}

	void ConditionVariable::broadcast() {
		if(destroyed)
			throw IllegalConditionVariableStateError(ConditionVariableOperationError::SIGNAL);
		mutex.lock();
		bool good = mutex.getLockDepth() > 1u && mutex.getOwner() == GetCurrentThreadId();
		mutex.unlock();
		if(!good)
			throw IllegalConditionLockStateError(ConditionVariableOperationError::SIGNAL);
		WakeAllConditionVariable(&handle);
	}

#endif /* Windows version differences */

	string ConditionVariable::getErrorMessage(ErrorCode error) {
		return Thread::getErrorMessage(error);
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

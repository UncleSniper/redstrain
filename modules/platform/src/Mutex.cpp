#include "MutexOperationError.hpp"
#include "IllegalMutexStateError.hpp"
#include "InconsistentMutexUnlockError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <string.h>
#endif /* OS-specific includes */

using std::string;

namespace redengine {
namespace platform {

	Mutex::Mutex(const Mutex&) {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	Mutex::Mutex() : destroyed(false), depth(0u) {
		pthread_mutexattr_t attrs;
		int err = pthread_mutexattr_init(&attrs);
		if(err)
			throw MutexOperationError(MutexOperationError::CREATE, err);
		err = pthread_mutexattr_settype(&attrs, PTHREAD_MUTEX_RECURSIVE);
		if(err) {
			pthread_mutexattr_destroy(&attrs);
			throw MutexOperationError(MutexOperationError::CREATE, err);
		}
		err = pthread_mutex_init(&handle, &attrs);
		if(err) {
			pthread_mutexattr_destroy(&attrs);
			throw MutexOperationError(MutexOperationError::CREATE, err);
		}
		err = pthread_mutexattr_destroy(&attrs);
		if(err) {
			pthread_mutex_destroy(&handle);
			throw MutexOperationError(MutexOperationError::CREATE, err);
		}
	}

	Mutex::~Mutex() {
		if(!destroyed)
			pthread_mutex_destroy(&handle);
	}

	void Mutex::lock() {
		if(destroyed)
			throw IllegalMutexStateError(MutexOperationError::LOCK);
		int err = pthread_mutex_lock(&handle);
		if(err)
			throw MutexOperationError(MutexOperationError::LOCK, err);
		++depth;
		owner = pthread_self();
	}

	bool Mutex::tryLock() {
		if(destroyed)
			throw IllegalMutexStateError(MutexOperationError::LOCK);
		int err = pthread_mutex_trylock(&handle);
		if(err) {
			if(err == EBUSY)
				return false;
			throw MutexOperationError(MutexOperationError::LOCK, err);
		}
		++depth;
		owner = pthread_self();
		return true;
	}

	void Mutex::unlock() {
		if(destroyed)
			throw IllegalMutexStateError(MutexOperationError::UNLOCK);
		int err = pthread_mutex_lock(&handle);
		if(err)
			throw MutexOperationError(MutexOperationError::UNLOCK, err);
		bool good = depth && owner == pthread_self();
		err = pthread_mutex_unlock(&handle);
		if(err)
			throw MutexOperationError(MutexOperationError::UNLOCK, err);
		if(!good)
			throw InconsistentMutexUnlockError();
		--depth;
		err = pthread_mutex_unlock(&handle);
		if(err) {
			++depth;
			throw MutexOperationError(MutexOperationError::UNLOCK, err);
		}
	}

	void Mutex::_uncheckedUnlock() {
		if(destroyed || !depth)
			return;
		--depth;
		if(pthread_mutex_unlock(&handle))
			++depth;
	}

	void Mutex::_aWinnerIsMe() {
		owner = pthread_self();
	}

	void Mutex::destroy() {
		if(destroyed)
			return;
		int err = pthread_mutex_destroy(&handle);
		if(err)
			throw MutexOperationError(MutexOperationError::DESTROY, err);
		destroyed = true;
	}

	string Mutex::getErrorMessage(ErrorCode error) {
		return strerror(error);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

#ifdef REDSTRAIN_PLATFORM_WINXP_COMPAT

	Mutex::Mutex() : destroyed(false), depth(0u) {
		handle = CreateMutex(NULL, FALSE, NULL);
		if(!handle)
			throw MutexOperationError(MutexOperationError::CREATE, GetLastError());
	}

	Mutex::~Mutex() {
		if(!destroyed)
			CloseHandle(handle);
	}

	void Mutex::lock() {
		if(destroyed)
			throw IllegalMutexStateError(MutexOperationError::LOCK);
		switch(WaitForSingleObject(handle, INFINITE)) {
			case WAIT_ABANDONED:
				if(WaitForSingleObject(handle, INFINITE) == WAIT_FAILED)
					throw MutexOperationError(MutexOperationError::LOCK, GetLastError());
				break;
			case WAIT_OBJECT_0:
				break;
			default:
				throw MutexOperationError(MutexOperationError::LOCK, GetLastError());
		}
		++depth;
		owner = GetCurrentThreadId();
	}

	bool Mutex::tryLock() {
		if(destroyed)
			throw IllegalMutexStateError(MutexOperationError::LOCK);
	  again:
		switch(WaitForSingleObject(handle, static_cast<DWORD>(0u))) {
			case WAIT_ABANDONED:
				goto again;
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				return false;
			default:
				throw MutexOperationError(MutexOperationError::LOCK, GetLastError());
		}
		++depth;
		owner = GetCurrentThreadId();
		return true;
	}

	void Mutex::unlock() {
		if(destroyed)
			throw IllegalMutexStateError(MutexOperationError::UNLOCK);
	  checkAgain:
		switch(WaitForSingleObject(handle, static_cast<DWORD>(0u))) {
			case WAIT_ABANDONED:
				goto checkAgain;
			case WAIT_OBJECT_0:
				break;
			default:
				throw MutexOperationError(MutexOperationError::LOCK, GetLastError());
		}
		bool good = depth && owner == GetCurrentThreadId();
		if(!ReleaseMutex(handle))
			throw MutexOperationError(MutexOperationError::LOCK, GetLastError());
		if(!good)
			throw InconsistentMutexUnlockError();
		--depth;
		if(!ReleaseMutex(handle)) {
			++depth;
			throw MutexOperationError(MutexOperationError::LOCK, GetLastError());
		}
	}

	void Mutex::_uncheckedUnlock() {
		if(destroyed || !depth)
			return;
		--depth;
		if(!ReleaseMutex(handle))
			++depth;
	}

	void Mutex::destroy() {
		if(destroyed)
			return;
		if(!CloseHandle(handle))
			throw MutexOperationError(MutexOperationError::DESTROY, GetLastError());
		destroyed = true;
	}

#else /* Vista or above */

	Mutex::Mutex() : destroyed(false), depth(0u) {
		InitializeCriticalSection(&handle);
	}

	Mutex::~Mutex() {
		if(!destroyed)
			DeleteCriticalSection(&handle);
	}

	void Mutex::lock() {
		if(destroyed)
			throw IllegalMutexStateError(MutexOperationError::LOCK);
		EnterCriticalSection(&handle);
		++depth;
		owner = GetCurrentThreadId();
	}

	bool Mutex::tryLock() {
		if(destroyed)
			throw IllegalMutexStateError(MutexOperationError::LOCK);
		if(!TryEnterCriticalSection(&handle))
			return false;
		++depth;
		owner = GetCurrentThreadId();
		return true;
	}

	void Mutex::unlock() {
		if(destroyed)
			throw IllegalMutexStateError(MutexOperationError::UNLOCK);
		EnterCriticalSection(&handle);
		bool good = depth && owner == GetCurrentThreadId();
		LeaveCriticalSection(&handle);
		if(!good)
			throw InconsistentMutexUnlockError();
		--depth;
		LeaveCriticalSection(&handle);
	}

	void Mutex::_uncheckedUnlock() {
		if(destroyed || !depth)
			return;
		--depth;
		LeaveCriticalSection(&handle);
	}

	void Mutex::destroy() {
		if(destroyed)
			return;
		DeleteCriticalSection(&handle);
		destroyed = true;
	}

#endif /* Windows version differences */

	void Mutex::_aWinnerIsMe() {
		owner = GetCurrentThreadId();
	}

	string Mutex::getErrorMessage(ErrorCode error) {
		return Thread::getErrorMessage(error);
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

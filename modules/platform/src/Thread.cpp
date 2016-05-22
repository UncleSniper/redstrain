#include <redstrain/util/IntegerBounds.hpp>

#include "MutexPool.hpp"
#include "ThreadOperationError.hpp"
#include "IllegalThreadStateError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#endif /* OS-specific includes */

using std::string;
using redengine::util::IntegerBounds;

namespace redengine {
namespace platform {

	void *const Thread::UNCAUGHT_EXCEPTION = const_cast<void**>(&Thread::UNCAUGHT_EXCEPTION);

	struct StartupInfo {

		Thread* thread;
		void* user;

	};

	Thread::Thread(Handle handle, State state) : handle(handle), state(state) {}

	Thread::Thread() : state(PRISTINE) {}

	Thread::Thread(const Thread& thread) : handle(thread.handle), state(thread.state) {}

	Thread::~Thread() {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	void* Thread::bootstrap(void* ptr) {
		StartupInfo* info = reinterpret_cast<StartupInfo*>(ptr);
		StartupInfo stacked(*info);
		delete info;
		try {
			return stacked.thread->runImpl(stacked.user);
		}
		catch(...) {
			return UNCAUGHT_EXCEPTION;
		}
	}

	void Thread::startImpl(void* user, bool detached) {
		if(state != PRISTINE)
			throw IllegalThreadStateError(ThreadOperationError::START, state);
		MutexPool::getDefaultMutexPool();
		StartupInfo* info = new StartupInfo;
		info->thread = this;
		info->user = user;
		pthread_attr_t attrs;
		int err = pthread_attr_init(&attrs);
		if(err) {
			delete info;
			throw ThreadOperationError(ThreadOperationError::START, err);
		}
		err = pthread_attr_setdetachstate(&attrs, detached ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);
		if(err) {
			delete info;
			pthread_attr_destroy(&attrs);
			throw ThreadOperationError(ThreadOperationError::START, err);
		}
		state = detached ? DETACHED : JOINABLE;
		err = pthread_create(&handle, &attrs, bootstrap, info);
		if(err) {
			state = PRISTINE;
			delete info;
			pthread_attr_destroy(&attrs);
			throw ThreadOperationError(ThreadOperationError::START, err);
		}
		err = pthread_attr_destroy(&attrs);
		if(err)
			throw ThreadOperationError(ThreadOperationError::START, err);
	}

	void* Thread::joinImpl() {
		if(state != JOINABLE)
			throw IllegalThreadStateError(ThreadOperationError::JOIN, state);
		void* result;
		int err = pthread_join(handle, &result);
		if(err)
			throw ThreadOperationError(ThreadOperationError::JOIN, err);
		state = TERMINATED;
		return result;
	}

	void Thread::kill() {
		if(state != JOINABLE && state != DETACHED)
			throw IllegalThreadStateError(ThreadOperationError::STOP, state);
		int err = pthread_kill(handle, SIGKILL);
		if(err)
			throw ThreadOperationError(ThreadOperationError::STOP, err);
		state = TERMINATED;
	}

	void Thread::interrupt() {
		if(state != JOINABLE && state != DETACHED)
			throw IllegalThreadStateError(ThreadOperationError::INTERRUPT, state);
		int err = pthread_kill(handle, SIGINT);
		if(err)
			throw ThreadOperationError(ThreadOperationError::INTERRUPT, err);
	}

	string Thread::getErrorMessage(ErrorCode error) {
		return strerror(error);
	}

	static const uint64_t SLEEP_BOUND = static_cast<uint64_t>(IntegerBounds<useconds_t>::MAX);
	static const uint64_t MAX_SLEEP = static_cast<uint64_t>(1000000u) < SLEEP_BOUND
			? static_cast<uint64_t>(1000000u) : SLEEP_BOUND;

	void Thread::sleep(uint64_t millis) {
		millis *= static_cast<uint64_t>(1000u);
		do {
			uint64_t chunk = millis;
			if(chunk > MAX_SLEEP)
				chunk = MAX_SLEEP;
			usleep(static_cast<useconds_t>(chunk));
			millis -= chunk;
		} while(millis);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	DWORD Thread::bootstrap(void* ptr) {
		StartupInfo* info = reinterpret_cast<StartupInfo*>(ptr);
		StartupInfo stacked(*info);
		delete info;
		try {
			stacked.thread->bodyResult = stacked.thread->runImpl(stacked.user);
			return static_cast<DWORD>(0u);
		}
		catch(...) {
			stacked.thread->bodyResult = UNCAUGHT_EXCEPTION;
			return static_cast<DWORD>(1u);
		}
	}

	void Thread::startImpl(void* user, bool) {
		if(state != PRISTINE)
			throw IllegalThreadStateError(ThreadOperationError::START, state);
		StartupInfo* info = new StartupInfo;
		info->thread = this;
		info->user = user;
		state = JOINABLE;
		HANDLE h = CreateThread(NULL, static_cast<SIZE_T>(0u), bootstrap, user,
				static_cast<DWORD>(0u), &handle);
		if(!h) {
			state = PRISTINE;
			throw ThreadOperationError(ThreadOperationError::START, GetLastError());
		}
		if(!CloseHandle(h))
			throw ThreadOperationError(ThreadOperationError::START, GetLastError());
	}

	void* Thread::joinImpl() {
		if(state != JOINABLE)
			throw IllegalThreadStateError(ThreadOperationError::JOIN, state);
		HANDLE h = OpenThread(SYNCHRONIZE, FALSE, handle);
		if(!h)
			throw ThreadOperationError(ThreadOperationError::JOIN, GetLastError());
		if(WaitForSingleObject(h, INFINITE) == WAIT_FAILED) {
			CloseHandle(h);
			throw ThreadOperationError(ThreadOperationError::JOIN, GetLastError());
		}
		DWORD status;
		if(!GetExitCodeThread(h, &status)) {
			CloseHandle(h);
			throw ThreadOperationError(ThreadOperationError::JOIN, GetLastError());
		}
		state = TERMINATED;
		if(!CloseHandle(h))
			throw ThreadOperationError(ThreadOperationError::JOIN, GetLastError());
		return bodyResult;
	}

	void Thread::kill() {
		if(state != JOINABLE)
			throw IllegalThreadStateError(ThreadOperationError::STOP, state);
		HANDLE h = OpenThread(THREAD_TERMINATE, FALSE, handle);
		if(!h)
			throw ThreadOperationError(ThreadOperationError::STOP, GetLastError());
		if(!TerminateThread(h, static_cast<DWORD>(2u))) {
			CloseHandle(h);
			throw ThreadOperationError(ThreadOperationError::STOP, GetLastError());
		}
		state = TERMINATED;
		if(!CloseHandle(h))
			throw ThreadOperationError(ThreadOperationError::STOP, GetLastError());
	}

	void Thread::interrupt() {
		if(state != JOINABLE)
			throw IllegalThreadStateError(ThreadOperationError::INTERRUPT, state);
	}

	string Thread::getErrorMessage(ErrorCode error) {
		char* buffer;
		if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
				error, static_cast<DWORD>(0u), reinterpret_cast<LPTSTR>(&buffer),
				static_cast<DWORD>(1u), NULL))
			return "Unknown error";
		string msg(buffer);
		LocalFree(buffer);
		return msg;
	}

	static const uint64_t MAX_SLEEP = static_cast<uint64_t>(IntegerBounds<DWORD>::MAX);

	void Thread::sleep(uint64_t millis) {
		do {
			uint64_t chunk = millis;
			if(chunk > MAX_SLEEP)
				chunk = MAX_SLEEP;
			Sleep(static_cast<DWORD>(chunk));
			millis -= chunk;
		} while(millis);
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

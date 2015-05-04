#include "StubThread.hpp"

namespace redengine {
namespace platform {

	StubThread::StubThread(Handle handle) {
			this->handle = handle;
			state = JOINABLE;
	}

	StubThread::StubThread(const StubThread& thread) : TypedThread<>(thread) {}

	void StubThread::run() {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	StubThread StubThread::self() {
		return StubThread(pthread_self());
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	StubThread StubThread::self() {
		return StubThread(GetCurrentThreadId());
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

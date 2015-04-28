#ifndef REDSTRAIN_MOD_PLATFORM_CONDITIONVARIABLE_HPP
#define REDSTRAIN_MOD_PLATFORM_CONDITIONVARIABLE_HPP

#include "Mutex.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ConditionVariable {

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef pthread_cond_t Handle;
		typedef int ErrorCode;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#ifdef REDSTRAIN_PLATFORM_WINXP_COMPAT
		// Using the strategy detailed in "Strategies for Implementing
		// POSIX Condition Variables on Win32" by Douglas C. Schmidt
		// and Irfan Pyarali, http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
		struct Handle {
			unsigned waitersCount;
			CRITICAL_SECTION waitersCountLock;
			HANDLE semaphore;
			HANDLE waitersDone;
			bool wasBroadcast;
		};
#else /* Vista or above */
		typedef CONDITION_VARIABLE Handle;
#endif /* Windows version differences */
		typedef DWORD ErrorCode;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

	  public:
		static const unsigned INFINITY = ~0u;

	  private:
		Handle handle;
		bool destroyed;

	  public:
		Mutex mutex;

	  private:
		ConditionVariable(const ConditionVariable&);

	  public:
		ConditionVariable();
		~ConditionVariable();

		bool wait(unsigned = INFINITY);
		void signal();
		void broadcast();

		inline bool isDestroyed() {
			return destroyed;
		}

		static std::string getErrorMessage(ErrorCode);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_CONDITIONVARIABLE_HPP */

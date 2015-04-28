#ifndef REDSTRAIN_MOD_PLATFORM_MUTEX_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEX_HPP

#include "Thread.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Mutex {

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef pthread_mutex_t Handle;
		typedef int ErrorCode;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#ifdef REDSTRAIN_PLATFORM_WINXP_COMPAT
		typedef HANDLE Handle;
#else /* Vista or above */
		typedef CRITICAL_SECTION Handle;
#endif /* Windows version differences */
		typedef DWORD ErrorCode;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

	  private:
		Handle handle;
		bool destroyed;
		volatile unsigned depth;
		volatile Thread::Handle owner;

	  private:
		Mutex(const Mutex&);

	  public:
		Mutex();
		~Mutex();

		inline const Handle& getHandle() const {
			return handle;
		}

		inline unsigned getLockDepth() const {
			return depth;
		}

		inline Thread::Handle getOwner() const {
			return owner;
		}

		void lock();
		bool tryLock();
		void unlock();
		void destroy();

		void _uncheckedUnlock();
		void _aWinnerIsMe();

		inline bool isDestroyed() {
			return destroyed;
		}

		static std::string getErrorMessage(ErrorCode);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEX_HPP */

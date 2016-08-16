#ifndef REDSTRAIN_MOD_PLATFORM_MUTEXLOCKER_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEXLOCKER_HPP

#include "Mutex.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API MutexLocker {

	  private:
		Mutex* mutex;

	  private:
		MutexLocker(const MutexLocker&);

	  public:
		MutexLocker(Mutex&);
		~MutexLocker();

		void release();
		void lock(Mutex&);

		inline bool isLocked() const {
			return !!mutex;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEXLOCKER_HPP */

#ifndef REDSTRAIN_MOD_PLATFORM_MUTEXLOCKER_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEXLOCKER_HPP

#include "Mutex.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API MutexLocker {

	  private:
		Mutex& mutex;
		bool unlocked;

	  private:
		MutexLocker(const MutexLocker&);

	  public:
		MutexLocker(Mutex&);
		~MutexLocker();

		void release();

		inline bool isLocked() const {
			return !unlocked;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEXLOCKER_HPP */

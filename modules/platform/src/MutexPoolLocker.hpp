#ifndef REDSTRAIN_MOD_PLATFORM_MUTEXPOOLLOCKER_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEXPOOLLOCKER_HPP

#include <string>

#include "MutexPool.hpp"

namespace redengine {
namespace platform {

	template<typename SubjectT>
	class MutexPoolLocker {

	  private:
		MutexPool& pool;
		const SubjectT& object;
		bool unlocked;

	  private:
		MutexPoolLocker(const MutexPoolLocker& locker) : pool(locker.pool), object(locker.object) {}

	  public:
		MutexPoolLocker(const SubjectT& object, MutexPool& pool) : pool(pool), object(object), unlocked(false) {
			pool.lockObject<SubjectT>(&object);
		}

		MutexPoolLocker(const SubjectT& object, MutexPool* pool = NULL)
				: pool(pool ? *pool : MutexPool::getDefaultMutexPool()), object(object), unlocked(false) {
			this->pool.lockObject<SubjectT>(&object);
		}

		~MutexPoolLocker() {
			if(!unlocked)
				pool.uncheckedUnlockObject<SubjectT>(&object);
		}

		void release() {
			if(!unlocked) {
				pool.unlockObject<SubjectT>(&object);
				unlocked = true;
			}
		}

		inline bool isLocked() const {
			return !unlocked;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEXPOOLLOCKER_HPP */

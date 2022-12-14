#ifndef REDSTRAIN_MOD_PLATFORM_OBJECTLOCKER_HPP
#define REDSTRAIN_MOD_PLATFORM_OBJECTLOCKER_HPP

#include <cstddef>

#include "MutexPool.hpp"

namespace redengine {
namespace platform {

	template<typename SubjectT>
	class ObjectLocker {

	  private:
		MutexPool& pool;
		const SubjectT* object;

	  private:
		ObjectLocker(const ObjectLocker& locker) : pool(locker.pool), object(locker.object) {}

	  public:
		ObjectLocker(const SubjectT* object) : pool(MutexPool::getDefaultMutexPool()), object(object) {
			pool.lockObject<SubjectT>(object);
		}

		ObjectLocker(MutexPool& pool, const SubjectT* object) : pool(pool), object(object) {
			pool.lockObject<SubjectT>(object);
		}

		~ObjectLocker() {
			pool.uncheckedUnlockObject<SubjectT>(object);
		}

		inline MutexPool& getMutexPool() {
			return pool;
		}

		inline const MutexPool& getMutexPool() const {
			return pool;
		}

		inline const SubjectT* getObject() const {
			return object;
		}

		void release() {
			pool.unlockObject<SubjectT>(object);
			object = NULL;
		}

		void move(const SubjectT* newObject) {
			if(newObject == object)
				return;
			pool.lockObject<SubjectT>(newObject);
			pool.unlockObject<SubjectT>(object);
			object = newObject;
		}

	};

	template<typename SubjectT>
	void lockAnyObject(const SubjectT* object) {
		MutexPool::getDefaultMutexPool().lockObject(object);
	}

	template<typename SubjectT>
	void unlockAnyObject(const SubjectT* object) {
		MutexPool::getDefaultMutexPool().unlockObject(object);
	}

	template<typename SubjectT>
	void uncheckedUnlockAnyObject(const SubjectT* object) {
		MutexPool::getDefaultMutexPool().uncheckedUnlockObject(object);
	}

}}

#endif /* REDSTRAIN_MOD_PLATFORM_OBJECTLOCKER_HPP */

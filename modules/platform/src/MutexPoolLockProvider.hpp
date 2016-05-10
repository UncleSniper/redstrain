#ifndef REDSTRAIN_MOD_PLATFORM_MUTEXPOOLLOCKPROVIDER_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEXPOOLLOCKPROVIDER_HPP

#include "MutexPool.hpp"
#include "LockProvider.hpp"

namespace redengine {
namespace platform {

	template<typename SubjectT>
	class MutexPoolLockProvider : public LockProvider<SubjectT> {

	  private:
		MutexPool& pool;
		bool managePool;

	  public:
		MutexPoolLockProvider() : pool(MutexPool::getDefaultMutexPool()), managePool(false) {}

		MutexPoolLockProvider(MutexPool& pool, bool managePool = false) : pool(pool), managePool(managePool) {}

		MutexPoolLockProvider(const MutexPoolLockProvider& provider)
				: LockProvider<SubjectT>(provider), pool(provider.pool), managePool(false) {}

		virtual ~MutexPoolLockProvider() {
			if(managePool)
				delete &pool;
		}

		inline MutexPool& getMutexPool() {
			return pool;
		}

		inline const MutexPool& getMutexPool() const {
			return pool;
		}

		inline bool isManagePool() const {
			return managePool;
		}

		inline void setManagePool(bool managePool) {
			this->managePool = managePool;
		}

		virtual void lockObject(const SubjectT* object) {
			pool.lockObject<SubjectT>(object);
		}

		virtual void unlockObject(const SubjectT* object) {
			pool.unlockObject<SubjectT>(object);
		}

		virtual void uncheckedUnlockObject(const SubjectT* object) {
			pool.uncheckedUnlockObject<SubjectT>(object);
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEXPOOLLOCKPROVIDER_HPP */

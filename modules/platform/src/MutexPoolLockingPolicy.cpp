#include "MutexPool.hpp"
#include "MutexPoolLockingPolicy.hpp"

namespace redengine {
namespace platform {

	MutexPoolLockingPolicy::MutexPoolLockingPolicy() : pool(MutexPool::getDefaultMutexPool()) {}

	MutexPoolLockingPolicy::MutexPoolLockingPolicy(LockingPolicyInitializer pool) : pool(pool) {}

	MutexPoolLockingPolicy::MutexPoolLockingPolicy(const MutexPoolLockingPolicy& policy) : pool(policy.pool) {}

	MutexPoolLockingPolicy::~MutexPoolLockingPolicy() {}

	void MutexPoolLockingPolicy::lockByPolicy() const {
		const_cast<MutexPool&>(pool).lockObject<MutexPoolLockingPolicy>(this);
	}

	void MutexPoolLockingPolicy::unlockByPolicy() const {
		const_cast<MutexPool&>(pool).unlockObject<MutexPoolLockingPolicy>(this);
	}

	void MutexPoolLockingPolicy::uncheckedUnlockByPolicy() const {
		const_cast<MutexPool&>(pool).uncheckedUnlockObject<MutexPoolLockingPolicy>(this);
	}

}}

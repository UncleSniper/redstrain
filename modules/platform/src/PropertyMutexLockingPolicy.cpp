#include "PropertyMutexLockingPolicy.hpp"

namespace redengine {
namespace platform {

	PropertyMutexLockingPolicy::PropertyMutexLockingPolicy() {}

	PropertyMutexLockingPolicy::PropertyMutexLockingPolicy(const PropertyMutexLockingPolicy&) {}

	void PropertyMutexLockingPolicy::lockByPolicy() const {
		const_cast<Mutex&>(mutex).lock();
	}

	void PropertyMutexLockingPolicy::unlockByPolicy() const {
		const_cast<Mutex&>(mutex).unlock();
	}

	void PropertyMutexLockingPolicy::uncheckedUnlockByPolicy() const {
		const_cast<Mutex&>(mutex)._uncheckedUnlock();
	}

}}

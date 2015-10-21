#include "NoLockingPolicy.hpp"

namespace redengine {
namespace platform {

	NoLockingPolicy::NoLockingPolicy() {}

	NoLockingPolicy::NoLockingPolicy(LockingPolicyInitializer) {}

	NoLockingPolicy::NoLockingPolicy(const NoLockingPolicy&) {}

	void NoLockingPolicy::lockByPolicy() const {}

	void NoLockingPolicy::unlockByPolicy() const {}

	void NoLockingPolicy::uncheckedUnlockByPolicy() const {}

}}

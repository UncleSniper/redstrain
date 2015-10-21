#ifndef REDSTRAIN_MOD_PLATFORM_MUTEXPOOLLOCKINGPOLICY_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEXPOOLLOCKINGPOLICY_HPP

#include "api.hpp"

namespace redengine {
namespace platform {

	class MutexPool;

	class REDSTRAIN_PLATFORM_API MutexPoolLockingPolicy {

	  public:
		typedef MutexPool& LockingPolicyInitializer;

	  private:
		MutexPool& pool;

	  public:
		MutexPoolLockingPolicy();
		MutexPoolLockingPolicy(LockingPolicyInitializer);
		MutexPoolLockingPolicy(const MutexPoolLockingPolicy&);
		virtual ~MutexPoolLockingPolicy();

		void lockByPolicy() const;
		void unlockByPolicy() const;
		void uncheckedUnlockByPolicy() const;

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEXPOOLLOCKINGPOLICY_HPP */

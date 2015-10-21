#ifndef REDSTRAIN_MOD_PLATFORM_PROPERTYMUTEXLOCKINGPOLICY_HPP
#define REDSTRAIN_MOD_PLATFORM_PROPERTYMUTEXLOCKINGPOLICY_HPP

#include "Mutex.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API PropertyMutexLockingPolicy {

	  private:
		struct REDSTRAIN_PLATFORM_API NoInitializer {};

	  public:
		typedef const NoInitializer& LockingPolicyInitializer;

	  private:
		Mutex mutex;

	  public:
		PropertyMutexLockingPolicy();
		PropertyMutexLockingPolicy(LockingPolicyInitializer);
		PropertyMutexLockingPolicy(const PropertyMutexLockingPolicy&);
		virtual ~PropertyMutexLockingPolicy();

		void lockByPolicy() const;
		void unlockByPolicy() const;
		void uncheckedUnlockByPolicy() const;

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_PROPERTYMUTEXLOCKINGPOLICY_HPP */

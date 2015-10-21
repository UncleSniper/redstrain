#ifndef REDSTRAIN_MOD_PLATFORM_NOLOCKINGPOLICY_HPP
#define REDSTRAIN_MOD_PLATFORM_NOLOCKINGPOLICY_HPP

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API NoLockingPolicy {

	  private:
		struct REDSTRAIN_PLATFORM_API NoInitializer {};

	  public:
		typedef const NoInitializer& LockingPolicyInitializer;

	  public:
		NoLockingPolicy();
		NoLockingPolicy(LockingPolicyInitializer);
		NoLockingPolicy(const NoLockingPolicy&);

		void lockByPolicy() const;
		void unlockByPolicy() const;
		void uncheckedUnlockByPolicy() const;

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_NOLOCKINGPOLICY_HPP */

#ifndef REDSTRAIN_MOD_PLATFORM_PROPERTYMUTEXLOCKINGPOLICY_HPP
#define REDSTRAIN_MOD_PLATFORM_PROPERTYMUTEXLOCKINGPOLICY_HPP

#include "Mutex.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API PropertyMutexLockingPolicy {

	  private:
		Mutex mutex;

	  public:
		PropertyMutexLockingPolicy();
		PropertyMutexLockingPolicy(const PropertyMutexLockingPolicy&);

		void lockByPolicy() const;
		void unlockByPolicy() const;
		void uncheckedUnlockByPolicy() const;

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_PROPERTYMUTEXLOCKINGPOLICY_HPP */

#ifndef REDSTRAIN_MOD_PLATFORM_PROVIDERLOCKER_HPP
#define REDSTRAIN_MOD_PLATFORM_PROVIDERLOCKER_HPP

#include <cstddef>

#include "LockProvider.hpp"

namespace redengine {
namespace platform {

	template<typename SubjectT>
	class ProviderLocker {

	  private:
		LockProvider<SubjectT>* provider;
		const SubjectT* object;
		bool unlocked;

	  private:
		ProviderLocker(const ProviderLocker&) {}

	  public:
		ProviderLocker(LockProvider<SubjectT>* provider, const SubjectT* object = NULL)
				: provider(provider), object(object), unlocked(false) {
			if(provider)
				provider->lockObject(object);
		}

		ProviderLocker(LockProvider<SubjectT>* provider, const SubjectT& object)
				: provider(provider), object(&object), unlocked(false) {
			if(provider)
				provider->lockObject(&object);
		}

		~ProviderLocker() {
			if(provider && !unlocked)
				provider->uncheckedUnlockObject(object);
		}

		void release() {
			if(!unlocked) {
				if(provider)
					provider->unlockObject(object);
				unlocked = true;
			}
		}

		ProviderLocker& operator=(const SubjectT* newObject) {
			if(provider && !unlocked) {
				provider->lockObject(newObject);
				provider->unlockObject(object);
			}
			object = newObject;
			return *this;
		}

		ProviderLocker& operator=(const ProviderLocker& locker) {
			if(locker.provider && !locker.unlocked)
				locker.provider->lockObject(locker.object);
			if(provider && !unlocked)
				provider->unlockObject(object);
			provider = locker.provider;
			object = locker.object;
			unlocked = locker.unlocked;
			return *this;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_PROVIDERLOCKER_HPP */

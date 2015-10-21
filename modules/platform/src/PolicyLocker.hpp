#ifndef REDSTRAIN_MOD_PLATFORM_POLICYLOCKER_HPP
#define REDSTRAIN_MOD_PLATFORM_POLICYLOCKER_HPP

#include <list>

namespace redengine {
namespace platform {

	template<typename SubjectT>
	class PolicyLocker {

	  private:
		const SubjectT* object;

	  public:
		PolicyLocker(const SubjectT* object = NULL) : object(object) {
			if(object)
				object->lockByPolicy();
		}

		PolicyLocker(const SubjectT& object) : object(&object) {
			object.lockByPolicy();
		}

		PolicyLocker(const PolicyLocker& locker) : object(locker.object) {
			if(object)
				object->lockByPolicy();
		}

		~PolicyLocker() {
			if(object)
				object->uncheckedUnlockByPolicy();
		}

		void release() {
			if(object) {
				object->unlockByPolicy();
				object = NULL;
			}
		}

		PolicyLocker& operator=(const SubjectT* newObject) {
			if(newObject)
				newObject->lockByPolicy();
			if(object)
				object->unlockByPolicy();
			object = newObject;
			return *this;
		}

		PolicyLocker& operator=(const PolicyLocker& locker) {
			if(locker.object)
				locker.object->lockByPolicy();
			if(object)
				object->unlockByPolicy();
			object = locker.object;
			return *this;
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_POLICYLOCKER_HPP */

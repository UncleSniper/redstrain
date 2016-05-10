#ifndef REDSTRAIN_MOD_PLATFORM_MUTEXLOCKPROVIDER_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEXLOCKPROVIDER_HPP

#include "Mutex.hpp"
#include "LockProvider.hpp"

namespace redengine {
namespace platform {

	template<typename SubjectT>
	class MutexLockProvider : public LockProvider<SubjectT> {

	  private:
		Mutex& mutex;
		bool manageMutex;

	  public:
		MutexLockProvider(Mutex& mutex, bool manageMutex = false) : mutex(mutex), manageMutex(manageMutex) {}

		MutexLockProvider(const MutexLockProvider& provider)
				: LockProvider<SubjectT>(provider), mutex(provider.mutex), manageMutex(false) {}

		virtual ~MutexLockProvider() {
			if(manageMutex)
				delete &mutex;
		}

		inline Mutex& getMutex() {
			return mutex;
		}

		inline const Mutex& getMutex() const {
			return mutex;
		}

		inline bool isManageMutex() const {
			return manageMutex;
		}

		inline void setManageMutex(bool manageMutex) {
			this->manageMutex = manageMutex;
		}

		virtual void lockObject(const SubjectT*) {
			mutex.lock();
		}

		virtual void unlockObject(const SubjectT*) {
			mutex.unlock();
		}

		virtual void uncheckedUnlockObject(const SubjectT*) {
			mutex._uncheckedUnlock();
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEXLOCKPROVIDER_HPP */

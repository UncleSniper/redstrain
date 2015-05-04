#include "MutexLocker.hpp"

namespace redengine {
namespace platform {

	MutexLocker::MutexLocker(const MutexLocker& locker) : mutex(locker.mutex) {}

	MutexLocker::MutexLocker(Mutex& mutex) : mutex(mutex), unlocked(false) {
		mutex.lock();
	}

	MutexLocker::~MutexLocker() {
		if(!unlocked)
			mutex._uncheckedUnlock();
	}

	void MutexLocker::release() {
		if(!unlocked) {
			mutex.unlock();
			unlocked = true;
		}
	}

}}

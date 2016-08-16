#include "MutexLocker.hpp"

namespace redengine {
namespace platform {

	MutexLocker::MutexLocker(const MutexLocker& locker) : mutex(locker.mutex) {}

	MutexLocker::MutexLocker(Mutex& mutex) : mutex(&mutex) {
		mutex.lock();
	}

	MutexLocker::~MutexLocker() {
		if(mutex)
			mutex->_uncheckedUnlock();
	}

	void MutexLocker::release() {
		if(mutex) {
			mutex->unlock();
			mutex = NULL;
		}
	}

	void MutexLocker::lock(Mutex& newMutex) {
		if(mutex) {
			mutex->unlock();
			mutex = NULL;
		}
		newMutex.lock();
		mutex = &newMutex;
	}

}}

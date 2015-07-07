#include <redstrain/util/MaxAlign.hpp>
#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/IntegerTypeByTraits.hpp>

#include "MutexLocker.hpp"
#include "ThreadingError.hpp"
#include "SuspendingMutexPool.hpp"
#include "tweaks.hpp"

using redengine::util::MaxAlign;
using redengine::util::IntegerLog;
using redengine::util::IntegerPointer;

namespace redengine {
namespace platform {

	SuspendingMutexPool::SuspendingMutexPool(unsigned size) {
		if(!size)
			size = REDSTRAIN_PLATFORM_SUSPENDING_MUTEX_POOL_DEFAULT_SIZE;
		this->size = size;
		shards = new Shard[size];
	}

	SuspendingMutexPool::SuspendingMutexPool(const SuspendingMutexPool& pool) : MutexPool(pool), size(pool.size) {
		shards = new Shard[pool.size];
	}

	SuspendingMutexPool::~SuspendingMutexPool() {
		delete[] shards;
	}

	static const size_t ALIGN_EXP = IntegerLog<size_t, static_cast<size_t>(2u), MaxAlign::MAX_ALIGNMENT>::EXPONENT;

	unsigned SuspendingMutexPool::indexByAddress(const void* object) const {
		IntegerPointer iptr = reinterpret_cast<IntegerPointer>(object) >> (ALIGN_EXP - static_cast<size_t>(1u));
		return static_cast<unsigned>(iptr % static_cast<IntegerPointer>(size));
	}

	void SuspendingMutexPool::lockObjectImpl(const void* object) {
		Shard& shard = shards[indexByAddress(object)];
		MutexLocker locker(shard.condvar.getMutex());
		for(;;) {
			if(shard.locked.insert(object).second) {
				// uncontended
				locker.release();
				return;
			}
			// somebody has the object locked;
			// wait for them to wake us after they unlock it
			shard.condvar.wait();
		}
	}

	void SuspendingMutexPool::unlockObjectImpl(const void* object) {
		Shard& shard = shards[indexByAddress(object)];
		MutexLocker locker(shard.condvar.getMutex());
		shard.locked.erase(object);
		shard.condvar.broadcast();
		locker.release();
	}

	void SuspendingMutexPool::uncheckedUnlockObjectImpl(const void* object) {
		try {
			unlockObjectImpl(object);
		}
		catch(const ThreadingError&) {}
	}

}}

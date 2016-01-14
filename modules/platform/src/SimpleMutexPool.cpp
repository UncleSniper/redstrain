#include <redstrain/util/MaxAlign.hpp>
#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/IntegerTypeByTraits.hpp>

#include "Mutex.hpp"
#include "SimpleMutexPool.hpp"
#include "tweaks.hpp"

using redengine::util::MaxAlign;
using redengine::util::MemorySize;
using redengine::util::IntegerLog;
using redengine::util::IntegerPointer;

namespace redengine {
namespace platform {

	SimpleMutexPool::SimpleMutexPool(unsigned size) {
		if(!size)
			size = REDSTRAIN_PLATFORM_SIMPLE_MUTEX_POOL_DEFAULT_SIZE;
		this->size = size;
		mutexes = new Mutex[size];
	}

	SimpleMutexPool::SimpleMutexPool(const SimpleMutexPool& pool) : MutexPool(pool), size(pool.size) {
		mutexes = new Mutex[pool.size];
	}

	SimpleMutexPool::~SimpleMutexPool() {
		delete[] mutexes;
	}

	static const MemorySize ALIGN_EXP = IntegerLog<
		MemorySize,
		static_cast<MemorySize>(2u),
		MaxAlign::MAX_ALIGNMENT
	>::EXPONENT;

	unsigned SimpleMutexPool::indexByAddress(const void* object) const {
		IntegerPointer iptr = reinterpret_cast<IntegerPointer>(object) >> (ALIGN_EXP - static_cast<MemorySize>(1u));
		return static_cast<unsigned>(iptr % static_cast<IntegerPointer>(size));
	}

	void SimpleMutexPool::lockObjectImpl(const void* object) {
		mutexes[indexByAddress(object)].lock();
	}

	void SimpleMutexPool::unlockObjectImpl(const void* object) {
		mutexes[indexByAddress(object)].unlock();
	}

	void SimpleMutexPool::uncheckedUnlockObjectImpl(const void* object) {
		mutexes[indexByAddress(object)]._uncheckedUnlock();
	}

}}

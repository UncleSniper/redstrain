#include "SuspendingMutexPool.hpp"

namespace redengine {
namespace platform {

	MutexPool::MutexPool() {}

	MutexPool::MutexPool(const MutexPool&) {}

	MutexPool::~MutexPool() {}

	static SuspendingMutexPool* defaultPool = NULL;

	MutexPool& MutexPool::getDefaultMutexPool() {
		if(!defaultPool)
			defaultPool = new SuspendingMutexPool;
		return *defaultPool;
	}

}}

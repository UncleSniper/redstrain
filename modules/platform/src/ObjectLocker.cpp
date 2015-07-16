#include "ObjectLocker.hpp"
#include "SimpleMutexPool.hpp"
#include "SuspendingMutexPool.hpp"

namespace redengine {
namespace platform {

	static SuspendingMutexPool* dgmp = NULL;

	REDSTRAIN_PLATFORM_API MutexPool& getDefaultGlobalMutexPool() {
		if(!dgmp)
			dgmp = new SuspendingMutexPool;
		return *dgmp;
	}

}}

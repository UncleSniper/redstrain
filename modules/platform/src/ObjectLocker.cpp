#include "ObjectLocker.hpp"
#include "SimpleMutexPool.hpp"
#include "SuspendingMutexPool.hpp"

namespace redengine {
namespace platform {

	static SuspendingMutexPool dgmp;

	REDSTRAIN_PLATFORM_API MutexPool& defaultGlobalMutexPool = dgmp;

}}

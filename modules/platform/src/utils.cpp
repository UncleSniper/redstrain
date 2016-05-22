#include <cstddef>
#include <stdint.h>

#include "platform.hpp"
#include "api.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <sys/time.h>
#endif /* OS-specific includes */

namespace redengine {
namespace platform {

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	REDSTRAIN_PLATFORM_API uint64_t currentTimeMillis() {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		tv.tv_usec /= static_cast<suseconds_t>(1000u);
		return static_cast<uint64_t>(tv.tv_sec) * static_cast<uint64_t>(1000u) + static_cast<uint64_t>(tv.tv_usec);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
//TODO
#error Windows is not supported yet.
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

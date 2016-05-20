#include "NativeRandom.hpp"
#include "platform.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <sys/time.h>
#include <sys/sysinfo.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
//TODO
#endif /* OS-specific includes */

using redengine::util::MemorySize;

namespace redengine {
namespace platform {

	NativeRandom NativeRandom::instance;

	NativeRandom::NativeRandom() {}

	NativeRandom::NativeRandom(const NativeRandom& random) : Random(random) {}

	MemorySize NativeRandom::getSeedSize() const {
		return static_cast<MemorySize>(0u);
	}

	void NativeRandom::seed(const char*) {}

	static void mix(char* begin, char* end, MemorySize start, const char* bytes, MemorySize count) {
		char* current = begin + start;
		for(; count; ++bytes, --count) {
			*current ^= *bytes;
			if(++current == end)
				current = begin;
		}
	}

	void NativeRandom::randomBytes(char* buffer, MemorySize size) {
		if(!size)
			return;
		char* end = buffer + size;
		MemorySize start = static_cast<MemorySize>(0u);
#define mixIt(thing) \
	mix(buffer, end, start, reinterpret_cast<const char*>(&(thing)), static_cast<MemorySize>(sizeof(thing))); \
	start = (start + static_cast<MemorySize>(1u)) % size;
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		struct timeval tv;
		if(!gettimeofday(&tv, NULL)) {
			mixIt(tv.tv_sec);
			mixIt(tv.tv_usec);
		}
		struct sysinfo si;
		if(!sysinfo(&si)) {
			mixIt(si.uptime);
			mixIt(*si.loads);
			mixIt(si.loads[1]);
			mixIt(si.loads[2]);
			mixIt(si.freeram);
			mixIt(si.bufferram);
			mixIt(si.freeswap);
			mixIt(si.procs);
		}
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#error Windows is not supported yet.
//TODO
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */
#undef mixIt
	}

}}

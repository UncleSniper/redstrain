#include "allocators.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace algorithm {

	REDSTRAIN_ALGORITHM_API void* standardAlloc(MemorySize size) {
		return ::operator new(static_cast<size_t>(size));
	}

	REDSTRAIN_ALGORITHM_API void standardFree(void* address) {
		::operator delete(address);
	}

}}

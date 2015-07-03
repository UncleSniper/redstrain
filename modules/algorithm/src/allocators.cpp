#include "allocators.hpp"

namespace redengine {
namespace algorithm {

	REDSTRAIN_ALGORITHM_API void* standardAlloc(size_t size) {
		return ::operator new(size);
	}

}}

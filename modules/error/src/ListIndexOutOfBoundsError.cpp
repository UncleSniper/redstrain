#include "ListIndexOutOfBoundsError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace error {

	ListIndexOutOfBoundsError::ListIndexOutOfBoundsError(MemorySize index) : IndexOutOfBoundsError(index) {}

	ListIndexOutOfBoundsError::ListIndexOutOfBoundsError(const ListIndexOutOfBoundsError& error)
			: Error(error), IndexOutOfBoundsError(error) {}

	REDSTRAIN_DEFINE_ERROR(ListIndexOutOfBoundsError) {
		out << "List index out of bounds: " << getOffendingIndex();
	}

}}

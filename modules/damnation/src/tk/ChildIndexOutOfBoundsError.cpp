#include "ChildIndexOutOfBoundsError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace tk {

	ChildIndexOutOfBoundsError::ChildIndexOutOfBoundsError(MemorySize index) : IndexOutOfBoundsError(index) {}

	ChildIndexOutOfBoundsError::ChildIndexOutOfBoundsError(const ChildIndexOutOfBoundsError& error)
			: Error(error), DamnError(error), IndexOutOfBoundsError(error) {}

	REDSTRAIN_DEFINE_ERROR(ChildIndexOutOfBoundsError) {
		out << "Child widget index out of bounds: " << getOffendingIndex();
	}

}}}

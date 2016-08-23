#include "TabIndexOutOfBoundsError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace tk {

	TabIndexOutOfBoundsError::TabIndexOutOfBoundsError(MemorySize index) : IndexOutOfBoundsError(index) {}

	TabIndexOutOfBoundsError::TabIndexOutOfBoundsError(const TabIndexOutOfBoundsError& error)
			: Error(error), DamnError(error), IndexOutOfBoundsError(error) {}

	REDSTRAIN_DEFINE_ERROR(TabIndexOutOfBoundsError) {
		out << "Tab index out of bounds: " << getOffendingIndex();
	}

}}}

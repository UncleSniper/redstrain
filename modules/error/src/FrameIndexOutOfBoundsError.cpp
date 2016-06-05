#include "FrameIndexOutOfBoundsError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace error {

	FrameIndexOutOfBoundsError::FrameIndexOutOfBoundsError(MemorySize index) : IndexOutOfBoundsError(index) {}

	FrameIndexOutOfBoundsError::FrameIndexOutOfBoundsError(const FrameIndexOutOfBoundsError& error)
			: Error(error), IndexOutOfBoundsError(error) {}

	REDSTRAIN_DEFINE_ERROR(FrameIndexOutOfBoundsError) {
		out << "Stack frame index out of bounds: " << getOffendingIndex();
	}

}}

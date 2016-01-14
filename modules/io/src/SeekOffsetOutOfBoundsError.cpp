#include "SeekOffsetOutOfBoundsError.hpp"

using redengine::util::FileOffset;

namespace redengine {
namespace io {

	SeekOffsetOutOfBoundsError::SeekOffsetOutOfBoundsError(FileOffset offset) : offset(offset) {}

	SeekOffsetOutOfBoundsError::SeekOffsetOutOfBoundsError(const SeekOffsetOutOfBoundsError& error)
			: Error(error), IOError(error), offset(error.offset) {}

	REDSTRAIN_DEFINE_ERROR(SeekOffsetOutOfBoundsError) {
		out << "Stream seek destination offset out of bounds: " << offset;
	}

}}

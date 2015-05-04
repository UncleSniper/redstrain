#include "SeekOffsetOutOfBoundsError.hpp"

namespace redengine {
namespace io {

	SeekOffsetOutOfBoundsError::SeekOffsetOutOfBoundsError(off_t offset) : offset(offset) {}

	SeekOffsetOutOfBoundsError::SeekOffsetOutOfBoundsError(const SeekOffsetOutOfBoundsError& error)
			: IOError(error), offset(error.offset) {}

	REDSTRAIN_DEFINE_ERROR(SeekOffsetOutOfBoundsError) {
		out << "Stream seek destination offset out of bounds: " << offset;
	}

}}

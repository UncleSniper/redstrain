#include "IllegalSeekWhenceError.hpp"

namespace redengine {
namespace io {

	IllegalSeekWhenceError::IllegalSeekWhenceError(Stream::SeekWhence whence) : whence(whence) {}

	IllegalSeekWhenceError::IllegalSeekWhenceError(const IllegalSeekWhenceError& error)
			: Error(error), IllegalArgumentError(error), whence(error.whence) {}

	REDSTRAIN_DEFINE_ERROR(IllegalSeekWhenceError) {
		out << "Not a valid SeekWhence: " << static_cast<int>(whence);
	}

}}

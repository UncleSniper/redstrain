#include "StreamBufferFullError.hpp"

namespace redengine {
namespace io {

	StreamBufferFullError::StreamBufferFullError() {}

	StreamBufferFullError::StreamBufferFullError(const StreamBufferFullError& error)
			: Error(error), IOError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(StreamBufferFullError) {
		out << "Stream buffer full";
	}

}}

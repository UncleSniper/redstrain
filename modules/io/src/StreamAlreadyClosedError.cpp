#include "StreamAlreadyClosedError.hpp"

namespace redengine {
namespace io {

	StreamAlreadyClosedError::StreamAlreadyClosedError() {}

	StreamAlreadyClosedError::StreamAlreadyClosedError(const StreamAlreadyClosedError& error)
			: Error(error), IOError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(StreamAlreadyClosedError) {
		out << "Stream has already been closed";
	}

}}

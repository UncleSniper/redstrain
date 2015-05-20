#include "StreamArrayClosedError.hpp"

namespace redengine {
namespace io {

	StreamArrayClosedError::StreamArrayClosedError() {}

	StreamArrayClosedError::StreamArrayClosedError(const StreamArrayClosedError& error)
			: Error(error), IOError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(StreamArrayClosedError) {
		out << "Stream-written array has already been closed";
	}

}}

#include "CompressionIOError.hpp"

namespace redengine {
namespace compress {

	CompressionIOError::CompressionIOError(CompressionError* cause) : cause(cause) {
		if(cause)
			cause->ref();
	}

	CompressionIOError::CompressionIOError(const CompressionIOError& error)
			: Error(error), IOError(error), cause(error.cause) {
		if(cause)
			cause->ref();
	}

	CompressionIOError::~CompressionIOError() {
		if(cause)
			cause->unref();
	}

	REDSTRAIN_DEFINE_ERROR(CompressionIOError) {
		out << "Error applying compressor to stream";
		if(cause) {
			out << ": ";
			cause->printMessage(out);
		}
	}

}}

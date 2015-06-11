#include "RenditionIOError.hpp"

namespace redengine {
namespace text {

	RenditionIOError::RenditionIOError(RenditionError* cause) : cause(cause) {
		if(cause)
			cause->ref();
	}

	RenditionIOError::RenditionIOError(const RenditionIOError& error)
			: Error(error), IOError(error), cause(error.cause) {
		if(cause)
			cause->ref();
	}

	RenditionIOError::~RenditionIOError() {
		if(cause)
			cause->unref();
	}

	REDSTRAIN_DEFINE_ERROR(RenditionIOError) {
		out << "Error applying text codec to stream";
		if(cause) {
			out << ": ";
			cause->printMessage(out);
		}
	}

}}

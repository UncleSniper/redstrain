#include "MappingLookupIOError.hpp"

using redengine::error::IOError;

namespace redengine {
namespace text {

	MappingLookupIOError::MappingLookupIOError(IOError* cause) : cause(cause) {
		if(cause)
			cause->ref();
	}

	MappingLookupIOError::MappingLookupIOError(const MappingLookupIOError& error)
			: MappingLookupError(error), cause(error.cause) {
		if(cause)
			cause->ref();
	}

	MappingLookupIOError::~MappingLookupIOError() {
		if(cause)
			cause->unref();
	}

	REDSTRAIN_DEFINE_ERROR(MappingLookupIOError) {
		out << "Error during character mapping lookup";
		if(cause) {
			out << ": ";
			cause->printMessage(out);
		}
	}

}}

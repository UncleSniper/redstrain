#include "ErrorHandlingError.hpp"

namespace redengine {
namespace error {

	ErrorHandlingError::ErrorHandlingError(Error* originalError, Error* subsequentError)
			: originalError(originalError), subsequentError(subsequentError) {
		if(originalError)
			originalError->ref();
		if(subsequentError)
			subsequentError->ref();
	}

	ErrorHandlingError::ErrorHandlingError(const ErrorHandlingError& error)
			: Error(error), originalError(error.originalError), subsequentError(error.subsequentError) {
		if(originalError)
			originalError->ref();
		if(subsequentError)
			subsequentError->ref();
	}

	ErrorHandlingError::~ErrorHandlingError() {
		if(originalError)
			originalError->unref();
		if(subsequentError)
			subsequentError->unref();
	}

	REDSTRAIN_DEFINE_ERROR(ErrorHandlingError) {
		out << "Error while handling error";
		if(subsequentError) {
			out << ": ";
			subsequentError->printMessage(out);
		}
	}

}}

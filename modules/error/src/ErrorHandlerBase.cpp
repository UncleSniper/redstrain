#include "Error.hpp"
#include "ErrorHandlerBase.hpp"

namespace redengine {
namespace error {

	ErrorHandlerBase::ErrorHandlerBase() : printStackTraces(Error::shouldPrintStackTraces()) {}

	ErrorHandlerBase::ErrorHandlerBase(const ErrorHandlerBase& handler)
			: ErrorHandler(handler), printStackTraces(handler.printStackTraces) {}

	void ErrorHandlerBase::handleError(const Error& error) {
		beginErrorType();
		writeErrorType(error.getErrorType());
		endErrorType();
		beginErrorMessage();
		writeErrorMessage(error);
		endErrorMessage();
		if(printStackTraces)
			writeStackTrace(error.getStackTrace());
	}

}}

#include "Error.hpp"
#include "ErrorHandlerBase8.hpp"

namespace redengine {
namespace error {

	ErrorHandlerBase8::ErrorHandlerBase8() {}

	ErrorHandlerBase8::ErrorHandlerBase8(const ErrorHandlerBase8& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase(handler) {}

	void ErrorHandlerBase8::beginErrorType() {
		prepareLine();
	}

	void ErrorHandlerBase8::writeErrorType(const char* typeName) {
		highlightErrorTypeName();
		writeString(typeName);
		endHighlighting();
	}

	void ErrorHandlerBase8::endErrorType() {}

	void ErrorHandlerBase8::beginErrorMessage() {
		writeString(": ");
	}

	void ErrorHandlerBase8::writeErrorMessage(const Error& error) {
		highlightErrorMessage();
		writeString(error.getMessage());
		endHighlighting();
	}

	void ErrorHandlerBase8::endErrorMessage() {
		endLine();
	}

}}

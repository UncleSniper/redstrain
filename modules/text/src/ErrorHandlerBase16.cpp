#include <redstrain/error/Error.hpp>
#include <redstrain/redmond/l10nbind.hpp>

#include "Transcode.hpp"
#include "ErrorHandlerBase16.hpp"

using redengine::error::Error;

namespace redengine {
namespace text {

	ErrorHandlerBase16::ErrorHandlerBase16() {}

	ErrorHandlerBase16::ErrorHandlerBase16(const ErrorHandlerBase16& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase(handler) {}

	void ErrorHandlerBase16::beginErrorType() {
		prepareLine();
	}

	void ErrorHandlerBase16::writeErrorType(const char* typeName) {
		highlightErrorTypeName();
		writeString(Transcode::utf8ToBMP(typeName));
		endHighlighting();
	}

	void ErrorHandlerBase16::endErrorType() {}

	static const String16 str_colonBlank(Transcode::utf8ToBMP(": "));

	void ErrorHandlerBase16::beginErrorMessage() {
		writeString(str_colonBlank);
	}

	void ErrorHandlerBase16::writeErrorMessage(const Error& error) {
		highlightErrorMessage();
		writeString(Transcode::utf8ToBMP(error.getMessage()));
		endHighlighting();
	}

	void ErrorHandlerBase16::endErrorMessage() {
		endLine();
	}

}}

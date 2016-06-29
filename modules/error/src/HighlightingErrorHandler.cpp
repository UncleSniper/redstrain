#include "HighlightingErrorHandler.hpp"

namespace redengine {
namespace error {

	HighlightingErrorHandler::HighlightingErrorHandler() {}

	HighlightingErrorHandler::HighlightingErrorHandler(const HighlightingErrorHandler& handler)
			: ErrorHandler(handler) {}

	void HighlightingErrorHandler::highlightErrorTypeSection() {}

	void HighlightingErrorHandler::highlightErrorTypeName() {}

	void HighlightingErrorHandler::highlightErrorMessage() {}

	void HighlightingErrorHandler::endHighlighting() {}

}}

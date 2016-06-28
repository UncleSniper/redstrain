#include "HighlightingStackTraceSink.hpp"

namespace redengine {
namespace error {

	HighlightingStackTraceSink::HighlightingStackTraceSink() {}

	HighlightingStackTraceSink::HighlightingStackTraceSink(const HighlightingStackTraceSink& sink)
			: StackTraceSink(sink) {}

	void HighlightingStackTraceSink::highlightNoTraceAvailable() {}

	void HighlightingStackTraceSink::highlightNoFramesOnStack() {}

	void HighlightingStackTraceSink::highlightBottomOfUnwinding() {}

	void HighlightingStackTraceSink::highlightFrameNotReconstructible() {}

	void HighlightingStackTraceSink::highlightMemoryAddress() {}

	void HighlightingStackTraceSink::highlightUnresolvedSymbol() {}

	void HighlightingStackTraceSink::highlightModuleName() {}

	void HighlightingStackTraceSink::endHighlighting() {}

}}

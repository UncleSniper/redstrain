#ifndef REDSTRAIN_MOD_ERROR_HIGHLIGHTINGSTACKTRACESINK_HPP
#define REDSTRAIN_MOD_ERROR_HIGHLIGHTINGSTACKTRACESINK_HPP

#include "StackTraceSink.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API HighlightingStackTraceSink : public virtual StackTraceSink {

	  protected:
		virtual void highlightNoTraceAvailable();
		virtual void highlightNoFrameOnStack();
		virtual void highlightBottomOfUnwinding();
		virtual void highlightFrameNotReconstructible();
		virtual void highlightMemoryAddress();
		virtual void highlightUnresolvedSymbol();
		virtual void highlightModuleName();
		virtual void endHighlighting();

	  public:
		HighlightingStackTraceSink();
		HighlightingStackTraceSink(const HighlightingStackTraceSink&);

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_HIGHLIGHTINGSTACKTRACESINK_HPP */

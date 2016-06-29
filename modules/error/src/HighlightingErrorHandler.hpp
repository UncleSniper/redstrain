#ifndef REDSTRAIN_MOD_ERROR_HIGHLIGHTINGERRORHANDLER_HPP
#define REDSTRAIN_MOD_ERROR_HIGHLIGHTINGERRORHANDLER_HPP

#include "ErrorHandler.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API HighlightingErrorHandler : public virtual ErrorHandler {

	  protected:
		virtual void highlightErrorTypeSection();
		virtual void highlightErrorTypeName();
		virtual void highlightErrorMessage();
		virtual void endHighlighting();

	  public:
		HighlightingErrorHandler();
		HighlightingErrorHandler(const HighlightingErrorHandler&);

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_HIGHLIGHTINGERRORHANDLER_HPP */

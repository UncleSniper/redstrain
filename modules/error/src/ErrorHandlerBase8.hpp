#ifndef REDSTRAIN_MOD_ERROR_ERRORHANDLERBASE8_HPP
#define REDSTRAIN_MOD_ERROR_ERRORHANDLERBASE8_HPP

#include <string>

#include "ErrorHandlerBase.hpp"
#include "ErrorHandlerTextBase.hpp"
#include "HighlightingErrorHandler.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API ErrorHandlerBase8
			: public ErrorHandlerBase, public virtual ErrorHandlerTextBase, public virtual HighlightingErrorHandler {

	  protected:
		virtual void beginErrorType();
		virtual void writeErrorType(const char*);
		virtual void endErrorType();
		virtual void beginErrorMessage();
		virtual void writeErrorMessage(const Error&);
		virtual void endErrorMessage();
		virtual void writeString(const std::string&) = 0;

	  public:
		ErrorHandlerBase8();
		ErrorHandlerBase8(const ErrorHandlerBase8&);

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_ERRORHANDLERBASE8_HPP */

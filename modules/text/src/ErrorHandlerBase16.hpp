#ifndef REDSTRAIN_MOD_TEXT_ERRORHANDLERBASE16_HPP
#define REDSTRAIN_MOD_TEXT_ERRORHANDLERBASE16_HPP

#include <redstrain/error/ErrorHandlerBase.hpp>
#include <redstrain/error/ErrorHandlerTextBase.hpp>
#include <redstrain/error/HighlightingErrorHandler.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API ErrorHandlerBase16 : public error::ErrorHandlerBase,
			public virtual error::ErrorHandlerTextBase, public virtual error::HighlightingErrorHandler {

	  protected:
		virtual void beginErrorType();
		virtual void writeErrorType(const char*);
		virtual void endErrorType();
		virtual void beginErrorMessage();
		virtual void writeErrorMessage(const error::Error&);
		virtual void endErrorMessage();
		virtual void writeString(const String16&) = 0;

	  public:
		ErrorHandlerBase16();
		ErrorHandlerBase16(const ErrorHandlerBase16&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_ERRORHANDLERBASE16_HPP */

#ifndef REDSTRAIN_MOD_ERROR_ERRORHANDLERTEXTBASE_HPP
#define REDSTRAIN_MOD_ERROR_ERRORHANDLERTEXTBASE_HPP

#include "ErrorHandler.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API ErrorHandlerTextBase : public virtual ErrorHandler {

	  protected:
		virtual void prepareLine() = 0;
		virtual void endLine() = 0;

	  public:
		ErrorHandlerTextBase();
		ErrorHandlerTextBase(const ErrorHandlerTextBase&);

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_ERRORHANDLERTEXTBASE_HPP */

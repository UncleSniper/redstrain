#ifndef REDSTRAIN_MOD_ERROR_ERRORHANDLERBASE_HPP
#define REDSTRAIN_MOD_ERROR_ERRORHANDLERBASE_HPP

#include "ErrorHandler.hpp"

namespace redengine {
namespace error {

	class StackTrace;

	class REDSTRAIN_ERROR_API ErrorHandlerBase : public virtual ErrorHandler {

	  private:
		bool printStackTraces;

	  protected:
		virtual void beginErrorType() = 0;
		virtual void writeErrorType(const char*) = 0;
		virtual void endErrorType() = 0;
		virtual void beginErrorMessage() = 0;
		virtual void writeErrorMessage(const Error&) = 0;
		virtual void endErrorMessage() = 0;
		virtual void writeStackTrace(const StackTrace*) = 0;

	  public:
		ErrorHandlerBase();
		ErrorHandlerBase(const ErrorHandlerBase&);

		inline bool shouldPrintStackTraces() const {
			return printStackTraces;
		}

		inline void setPrintStackTraces(bool printStackTraces) {
			this->printStackTraces = printStackTraces;
		}

		virtual void handleError(const Error&);

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_ERRORHANDLERBASE_HPP */

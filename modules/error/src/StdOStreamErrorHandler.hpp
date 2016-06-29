#ifndef REDSTRAIN_MOD_ERROR_STDOSTREAMERRORHANDLER_HPP
#define REDSTRAIN_MOD_ERROR_STDOSTREAMERRORHANDLER_HPP

#include <iostream>

#include "ErrorHandlerBase8.hpp"
#include "IndentingErrorHandler.hpp"

namespace redengine {
namespace error {

	class StdOStreamStackTraceSink;
	class StdOStreamStackTraceIndenter;

	class REDSTRAIN_ERROR_API StdOStreamErrorHandler : public ErrorHandlerBase8, public IndentingErrorHandler {

	  private:
		std::ostream& stream;

	  protected:
		virtual void writeString(const std::string&);
		virtual void writeStackTrace(const StackTrace*);
		virtual void configureStackTraceSink(StdOStreamStackTraceSink&, StdOStreamStackTraceIndenter&);

	  public:
		StdOStreamErrorHandler(std::ostream&, Indenter&);
		StdOStreamErrorHandler(const StdOStreamErrorHandler&);

	  public:
		static StdOStreamErrorHandler defaultStdErrorHandler;

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_STDOSTREAMERRORHANDLER_HPP */

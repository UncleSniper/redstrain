#include "StackTrace.hpp"
#include "StdOStreamErrorHandler.hpp"
#include "StdOStreamErrorIndenter.hpp"
#include "StdOStreamStackTraceSink.hpp"
#include "StdOStreamStackTraceIndenter.hpp"

using std::cerr;
using std::string;
using std::ostream;

namespace redengine {
namespace error {

	static StdOStreamErrorIndenter defaultStdErrorIndenter(cerr);

	StdOStreamErrorHandler StdOStreamErrorHandler::defaultStdErrorHandler(cerr, defaultStdErrorIndenter);

	StdOStreamErrorHandler::StdOStreamErrorHandler(ostream& stream, Indenter& indenter)
			: IndentingErrorHandler(indenter), stream(stream) {}

	StdOStreamErrorHandler::StdOStreamErrorHandler(const StdOStreamErrorHandler& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase8(handler), IndentingErrorHandler(handler), stream(handler.stream) {}

	void StdOStreamErrorHandler::writeString(const string& data) {
		stream << data;
	}

	void StdOStreamErrorHandler::writeStackTrace(const StackTrace* trace) {
		StdOStreamStackTraceIndenter indenter(stream);
		StdOStreamStackTraceSink sink(stream, indenter);
		configureStackTraceSink(sink, indenter);
		if(trace)
			trace->printTo(sink);
		else
			sink.noStackTraceAvailable();
	}

	void StdOStreamErrorHandler::configureStackTraceSink(StdOStreamStackTraceSink&, StdOStreamStackTraceIndenter&) {}

}}

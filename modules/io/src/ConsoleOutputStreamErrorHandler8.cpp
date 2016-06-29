#include <redstrain/error/StackTrace.hpp>

#include "OutputStreamStackTraceIndenter8.hpp"
#include "ConsoleOutputStreamErrorHandler8.hpp"
#include "ConsoleOutputStreamStackTraceSink8.hpp"

using redengine::platform::Console;
using redengine::error::StackTrace;

namespace redengine {
namespace io {

	ConsoleOutputStreamErrorHandler8::ConsoleOutputStreamErrorHandler8(OutputStream<char>& stream,
			Indenter& indenter, Console* console)
			: OutputStreamErrorHandler8(stream, indenter), ConsoleHighlightingErrorHandler(console) {}

	ConsoleOutputStreamErrorHandler8::ConsoleOutputStreamErrorHandler8(const
			ConsoleOutputStreamErrorHandler8& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			OutputStreamErrorHandler8(handler), ConsoleHighlightingErrorHandler(handler) {}

	void ConsoleOutputStreamErrorHandler8::writeStackTrace(const StackTrace* trace) {
		OutputStreamStackTraceIndenter8 indenter(getStream());
		indenter.setIndentationChain(this);
		ConsoleOutputStreamStackTraceSink8 sink(getStream(), indenter, getConsole());
		configureStackTraceSink(sink, indenter);
		configureConsoleStackTraceSink(sink, indenter);
		if(trace)
			trace->printTo(sink);
		else
			sink.noStackTraceAvailable();
	}

	void ConsoleOutputStreamErrorHandler8::configureConsoleStackTraceSink(ConsoleOutputStreamStackTraceSink8&,
			OutputStreamStackTraceIndenter8&) {}

}}

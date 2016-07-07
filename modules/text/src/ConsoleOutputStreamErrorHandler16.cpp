#include <redstrain/error/StackTrace.hpp>

#include "OutputStreamStackTraceIndenter16.hpp"
#include "ConsoleOutputStreamErrorHandler16.hpp"
#include "ConsoleOutputStreamStackTraceSink16.hpp"

using redengine::io::OutputStream;
using redengine::platform::Console;
using redengine::error::StackTrace;

namespace redengine {
namespace text {

	ConsoleOutputStreamErrorHandler16::ConsoleOutputStreamErrorHandler16(OutputStream<Char16>& stream,
			Indenter& indenter, Console* console)
			: OutputStreamErrorHandler16(stream, indenter), ConsoleHighlightingErrorHandler(console) {}

	ConsoleOutputStreamErrorHandler16::ConsoleOutputStreamErrorHandler16(const
			ConsoleOutputStreamErrorHandler16& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase16(handler), OutputStreamErrorHandler16(handler),
			ConsoleHighlightingErrorHandler(handler) {}

	void ConsoleOutputStreamErrorHandler16::writeStackTrace(const StackTrace* trace) {
		OutputStreamStackTraceIndenter16 indenter(getStream());
		ConsoleOutputStreamStackTraceSink16 sink(getStream(), indenter, getConsole());
		sink.setConsole(sink.getConsole());   // trigger notifyConsoleChanged() to update columnCount
		sink.advanceCurrentColumn(getCurrentColumn());
		configureStackTraceSink(sink, indenter);
		configureConsoleStackTraceSink(sink, indenter);
		if(trace)
			trace->printTo(sink);
		else
			sink.noStackTraceAvailable();
	}

	void ConsoleOutputStreamErrorHandler16::configureConsoleStackTraceSink(ConsoleOutputStreamStackTraceSink16&,
			OutputStreamStackTraceIndenter16&) {}

}}

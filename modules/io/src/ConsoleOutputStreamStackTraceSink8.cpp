#include "OutputStreamSymbolIndenter8.hpp"
#include "ConsoleOutputStreamSymbolSink8.hpp"
#include "ConsoleOutputStreamStackTraceSink8.hpp"

using redengine::platform::Console;
using redengine::redmond::unmangle::CPPSymbol;

namespace redengine {
namespace io {

	ConsoleOutputStreamStackTraceSink8::ConsoleOutputStreamStackTraceSink8(OutputStream<char>& stream,
			Indenter& indenter, Console* console)
			: OutputStreamStackTraceSink8(stream, indenter), ConsoleHighlightingStackTraceSink(console) {}

	ConsoleOutputStreamStackTraceSink8::ConsoleOutputStreamStackTraceSink8(const
			ConsoleOutputStreamStackTraceSink8& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			OutputStreamStackTraceSink8(sink), ConsoleHighlightingStackTraceSink(sink) {}

	void ConsoleOutputStreamStackTraceSink8::writeSymbol(const CPPSymbol& symbol) {
		OutputStreamSymbolIndenter8 indenter(getStream());
		indenter.setIndentationChain(this);
		ConsoleOutputStreamSymbolSink8 sink(getStream(), indenter, getConsole());
		sink.setConsole(sink.getConsole());   // trigger notifyConsoleChanged() to update columnCount
		sink.advanceCurrentColumn(getCurrentColumn());
		configureSymbolSink(sink, indenter);
		configureConsoleSymbolSink(sink, indenter);
		symbol.print(sink);
	}

	void ConsoleOutputStreamStackTraceSink8::configureConsoleSymbolSink(ConsoleOutputStreamSymbolSink8&,
			OutputStreamSymbolIndenter8&) {}

}}

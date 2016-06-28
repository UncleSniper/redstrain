#include "OutputStreamSymbolIndenter16.hpp"
#include "ConsoleOutputStreamSymbolSink16.hpp"
#include "ConsoleOutputStreamStackTraceSink16.hpp"

using redengine::io::OutputStream;
using redengine::platform::Console;
using redengine::redmond::unmangle::CPPSymbol;

namespace redengine {
namespace text {

	ConsoleOutputStreamStackTraceSink16::ConsoleOutputStreamStackTraceSink16(OutputStream<Char16>& stream,
			Indenter& indenter, Console* console)
			: OutputStreamStackTraceSink16(stream, indenter), ConsoleHighlightingStackTraceSink(console) {}

	ConsoleOutputStreamStackTraceSink16::ConsoleOutputStreamStackTraceSink16(const
			ConsoleOutputStreamStackTraceSink16& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			StackTraceSinkBase16(sink), OutputStreamStackTraceSink16(sink), ConsoleHighlightingStackTraceSink(sink) {}

	void ConsoleOutputStreamStackTraceSink16::writeSymbol(const CPPSymbol& symbol) {
		OutputStreamSymbolIndenter16 indenter(getStream());
		ConsoleOutputStreamSymbolSink16 sink(getStream(), indenter, getConsole());
		configureSymbolSink(sink, indenter);
		configureConsoleSymbolSink(sink, indenter);
		symbol.print(sink);
	}

	void ConsoleOutputStreamStackTraceSink16::configureConsoleSymbolSink(ConsoleOutputStreamSymbolSink16&,
			OutputStreamSymbolIndenter16&) {}

}}

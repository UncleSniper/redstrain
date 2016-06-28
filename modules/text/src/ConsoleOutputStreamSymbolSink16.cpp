#include "ConsoleOutputStreamSymbolSink16.hpp"

using redengine::io::OutputStream;
using redengine::platform::Console;

namespace redengine {
namespace text {

	ConsoleOutputStreamSymbolSink16::ConsoleOutputStreamSymbolSink16(OutputStream<Char16>& stream,
			Indenter& indenter, Console* console, int flags) : OutputStreamSymbolSink16(stream, indenter, flags),
			ConsoleHighlightingSymbolSink(console) {}

	ConsoleOutputStreamSymbolSink16::ConsoleOutputStreamSymbolSink16(const ConsoleOutputStreamSymbolSink16& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), OutputStreamSymbolSink16(sink),
			ConsoleHighlightingSymbolSink(sink) {}

	void ConsoleOutputStreamSymbolSink16::notifyConsoleChanged() {
		if(getConsole()) {
			unsigned width, height;
			Console::getConsoleDimensions(width, height);
			setColumnCount(width);
		}
		else
			setColumnCount(0u);
	}

}}

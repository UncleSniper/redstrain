#include "ConsoleOutputStreamSymbolSink8.hpp"

using redengine::platform::Console;

namespace redengine {
namespace io {

	ConsoleOutputStreamSymbolSink8::ConsoleOutputStreamSymbolSink8(OutputStream<char>& stream, Indenter& indenter,
			Console* console, int flags) : OutputStreamSymbolSink8(stream, indenter, flags),
			ConsoleHighlightingSymbolSink(console) {}

	ConsoleOutputStreamSymbolSink8::ConsoleOutputStreamSymbolSink8(const ConsoleOutputStreamSymbolSink8& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), OutputStreamSymbolSink8(sink),
			ConsoleHighlightingSymbolSink(sink) {}

	void ConsoleOutputStreamSymbolSink8::notifyConsoleChanged() {
		if(getConsole()) {
			unsigned width, height;
			Console::getConsoleDimensions(width, height);
			setColumnCount(width);
		}
		else
			setColumnCount(0u);
	}

}}

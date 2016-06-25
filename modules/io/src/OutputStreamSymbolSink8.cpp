#include "OutputStreamSymbolSink8.hpp"

using std::string;
using redengine::platform::Console;

namespace redengine {
namespace io {

	OutputStreamSymbolSink8::OutputStreamSymbolSink8(OutputStream<char>& stream, Indenter& indenter,
			Console* console, int flags) : SymbolSinkBase8(flags), BoundedSymbolSink(0u),
			IndentingSymbolSink(indenter), ConsoleHighlightingSymbolSink(console),
			stream(stream), formatted(stream), lastWasGreater(false), lastWasLess(false) {}

	OutputStreamSymbolSink8::OutputStreamSymbolSink8(const OutputStreamSymbolSink8& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), SymbolSinkBase8(sink), BoundedSymbolSink(sink),
			IndentingSymbolSink(sink), ConsoleHighlightingSymbolSink(sink), stream(sink.stream), formatted(stream),
			lastWasGreater(sink.lastWasGreater), lastWasLess(sink.lastWasLess) {}

	void OutputStreamSymbolSink8::writeString(const string& data) {
		if(data.empty())
			return;
		char fc = data[static_cast<string::size_type>(0u)];
		char lc = data[data.length() - static_cast<string::size_type>(1u)];
		unsigned length = static_cast<unsigned>(data.length());
		if((fc == '>' && lastWasGreater) || (fc == '<' && lastWasLess)) {
			formatted.print(' ');
			++length;
		}
		formatted.print(data);
		lastWasGreater = lc == '>';
		lastWasLess = lc == '<';
		advanceCurrentColumn(length);
	}

	void OutputStreamSymbolSink8::notifyIndented(unsigned columns) {
		resetCurrentColumn();
		advanceCurrentColumn(columns);
	}

	void OutputStreamSymbolSink8::notifyConsoleChanged() {
		if(getConsole()) {
			unsigned width, height;
			Console::getConsoleDimensions(width, height);
			setColumnCount(width);
		}
		else
			setColumnCount(0u);
	}

	void OutputStreamSymbolSink8::startNewLine(int levelDelta) {
		IndentingSymbolSink::startNewLine(levelDelta);
		lastWasGreater = lastWasLess = false;
	}

}}

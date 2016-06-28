#include "OutputStreamSymbolSink16.hpp"

using redengine::io::OutputStream;

namespace redengine {
namespace text {

	OutputStreamSymbolSink16::OutputStreamSymbolSink16(OutputStream<Char16>& stream, Indenter& indenter, int flags)
			: SymbolSinkBase16(flags), BoundedSymbolSink(0u), IndentingSymbolSink(indenter),
			stream(stream), formatted(stream), lastWasGreater(false), lastWasLess(false) {}

	OutputStreamSymbolSink16::OutputStreamSymbolSink16(const OutputStreamSymbolSink16& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), SymbolSinkBase16(sink), BoundedSymbolSink(sink),
			IndentingSymbolSink(sink), stream(sink.stream), formatted(stream),
			lastWasGreater(sink.lastWasGreater), lastWasLess(sink.lastWasLess) {}

	void OutputStreamSymbolSink16::writeString(const String16& data) {
		if(data.empty())
			return;
		Char16 fc = data[static_cast<String16::size_type>(0u)];
		Char16 lc = data[data.length() - static_cast<String16::size_type>(1u)];
		unsigned length = static_cast<unsigned>(data.length());
		if((fc == static_cast<Char16>('>') && lastWasGreater) || (fc == static_cast<Char16>('<') && lastWasLess)) {
			formatted.print(static_cast<Char16>(' '));
			++length;
		}
		formatted.print(data);
		lastWasGreater = lc == static_cast<Char16>('>');
		lastWasLess = lc == static_cast<Char16>('<');
		advanceCurrentColumn(length);
	}

	void OutputStreamSymbolSink16::notifyIndented(unsigned columns) {
		resetCurrentColumn();
		advanceCurrentColumn(columns);
	}

	void OutputStreamSymbolSink16::startNewLine(int levelDelta) {
		IndentingSymbolSink::startNewLine(levelDelta);
		lastWasGreater = lastWasLess = false;
	}

}}

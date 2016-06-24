#include "StdOStreamSymbolSink.hpp"

using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	StdOStreamSymbolSink::StdOStreamSymbolSink(ostream& stream, Indenter& indenter, int flags, unsigned columnCount)
			: SymbolSinkBase8(flags), BoundedSymbolSink(columnCount), IndentingSymbolSink(indenter),
			stream(stream), lastWasGreater(false), lastWasLess(false) {}

	StdOStreamSymbolSink::StdOStreamSymbolSink(const StdOStreamSymbolSink& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), SymbolSinkBase8(sink), BoundedSymbolSink(sink),
			IndentingSymbolSink(sink), stream(sink.stream), lastWasGreater(sink.lastWasGreater),
			lastWasLess(sink.lastWasLess) {}

	void StdOStreamSymbolSink::writeString(const string& data) {
		if(data.empty())
			return;
		char fc = data[static_cast<string::size_type>(0u)];
		char lc = data[data.length() - static_cast<string::size_type>(1u)];
		unsigned length = static_cast<unsigned>(data.length());
		if((fc == '>' && lastWasGreater) || (fc == '<' && lastWasLess)) {
			stream << ' ';
			++length;
		}
		stream << data;
		lastWasGreater = lc == '>';
		lastWasLess = lc == '<';
		advanceCurrentColumn(static_cast<unsigned>(data.length()));
	}

	void StdOStreamSymbolSink::notifyIndented(unsigned columns) {
		resetCurrentColumn();
		advanceCurrentColumn(columns);
	}

}}}

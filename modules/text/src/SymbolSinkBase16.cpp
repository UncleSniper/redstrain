#include "Transcode.hpp"
#include "SymbolSinkBase16.hpp"

using std::string;

namespace redengine {
namespace text {

	SymbolSinkBase16::SymbolSinkBase16(int flags) : SymbolSinkBase8(flags) {}

	SymbolSinkBase16::SymbolSinkBase16(const SymbolSinkBase16& sink)
			: SymbolSink(sink), HighlightingSymbolSink(sink), SymbolSinkBase8(sink) {}

	void SymbolSinkBase16::writeString(const string& data) {
		writeString(Transcode::utf8ToBMP(data));
	}

}}

#include "OutputStreamSymbolIndenter8.hpp"

using std::string;

namespace redengine {
namespace io {

	OutputStreamSymbolIndenter8::OutputStreamSymbolIndenter8(OutputStream<char>& stream)
			: stream(stream), formatted(stream) {}

	OutputStreamSymbolIndenter8::OutputStreamSymbolIndenter8(const OutputStreamSymbolIndenter8& indenter)
			: SymbolSinkIndenterBase8(indenter), stream(indenter.stream), formatted(stream) {}

	OutputStreamSymbolIndenter8::~OutputStreamSymbolIndenter8() {}

	void OutputStreamSymbolIndenter8::writeString(const string& data) {
		formatted.print(data);
	}

	void OutputStreamSymbolIndenter8::startNewLine() {
		formatted.endLine();
	}

}}

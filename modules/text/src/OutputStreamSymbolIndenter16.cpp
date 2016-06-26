#include "OutputStreamSymbolIndenter16.hpp"

using redengine::io::OutputStream;

namespace redengine {
namespace text {

	OutputStreamSymbolIndenter16::OutputStreamSymbolIndenter16(OutputStream<Char16>& stream)
			: stream(stream), formatted(stream) {}

	OutputStreamSymbolIndenter16::OutputStreamSymbolIndenter16(const OutputStreamSymbolIndenter16& indenter)
			: SymbolSinkIndenterBase16(indenter), stream(indenter.stream), formatted(stream) {}

	void OutputStreamSymbolIndenter16::writeString(const String16& data) {
		formatted.print(data);
	}

	void OutputStreamSymbolIndenter16::startNewLine() {
		formatted.endLine();
	}

}}

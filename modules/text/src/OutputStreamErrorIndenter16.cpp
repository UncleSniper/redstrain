#include "OutputStreamErrorIndenter16.hpp"

using redengine::io::OutputStream;

namespace redengine {
namespace text {

	OutputStreamErrorIndenter16::OutputStreamErrorIndenter16(OutputStream<Char16>& stream)
			: stream(stream), formatted(stream) {}

	OutputStreamErrorIndenter16::OutputStreamErrorIndenter16(const OutputStreamErrorIndenter16& indenter)
			: ErrorIndenterBase16(indenter), stream(indenter.stream), formatted(stream) {}

	OutputStreamErrorIndenter16::~OutputStreamErrorIndenter16() {}

	void OutputStreamErrorIndenter16::writeString(const String16& data) {
		formatted.print(data);
	}

	void OutputStreamErrorIndenter16::endLine() {
		formatted.endLine();
	}

}}

#include "OutputStreamStackTraceIndenter16.hpp"

using redengine::io::OutputStream;

namespace redengine {
namespace text {

	OutputStreamStackTraceIndenter16::OutputStreamStackTraceIndenter16(OutputStream<Char16>& stream)
			: stream(stream), formatted(stream) {}

	OutputStreamStackTraceIndenter16::OutputStreamStackTraceIndenter16(const
			OutputStreamStackTraceIndenter16& indenter)
			: StackTraceIndenterBase16(indenter), stream(indenter.stream), formatted(stream) {}

	void OutputStreamStackTraceIndenter16::writeString(const String16& data) {
		formatted.print(data);
	}

	void OutputStreamStackTraceIndenter16::endLine() {
		formatted.endLine();
	}

}}

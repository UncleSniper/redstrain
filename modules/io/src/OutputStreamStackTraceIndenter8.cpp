#include "OutputStreamStackTraceIndenter8.hpp"

using std::string;

namespace redengine {
namespace io {

	OutputStreamStackTraceIndenter8::OutputStreamStackTraceIndenter8(OutputStream<char>& stream)
			: stream(stream), formatted(stream) {}

	OutputStreamStackTraceIndenter8::OutputStreamStackTraceIndenter8(const OutputStreamStackTraceIndenter8& indenter)
			: StackTraceIndenterBase8(indenter), stream(indenter.stream), formatted(stream) {}

	void OutputStreamStackTraceIndenter8::writeString(const string& data) {
		formatted.print(data);
	}

	void OutputStreamStackTraceIndenter8::endLine() {
		formatted.endLine();
	}

}}

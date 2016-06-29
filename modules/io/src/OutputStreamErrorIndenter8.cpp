#include "OutputStreamErrorIndenter8.hpp"

using std::string;

namespace redengine {
namespace io {

	OutputStreamErrorIndenter8::OutputStreamErrorIndenter8(OutputStream<char>& stream)
			: stream(stream), formatted(stream) {}

	OutputStreamErrorIndenter8::OutputStreamErrorIndenter8(const OutputStreamErrorIndenter8& indenter)
			: ErrorIndenterBase8(indenter), stream(indenter.stream), formatted(stream) {}

	OutputStreamErrorIndenter8::~OutputStreamErrorIndenter8() {}

	void OutputStreamErrorIndenter8::writeString(const string& data) {
		formatted.print(data);
	}

	void OutputStreamErrorIndenter8::endLine() {
		formatted.endLine();
	}

}}

#include "StdOStreamStackTraceIndenter.hpp"

using std::endl;
using std::string;
using std::ostream;

namespace redengine {
namespace error {

	StdOStreamStackTraceIndenter::StdOStreamStackTraceIndenter(ostream& stream) : stream(stream) {}

	StdOStreamStackTraceIndenter::StdOStreamStackTraceIndenter(const StdOStreamStackTraceIndenter& indenter)
			: StackTraceIndenterBase8(indenter), stream(indenter.stream) {}

	void StdOStreamStackTraceIndenter::writeString(const string& data) {
		stream << data;
	}

	void StdOStreamStackTraceIndenter::endLine() {
		stream << endl;
	}

}}

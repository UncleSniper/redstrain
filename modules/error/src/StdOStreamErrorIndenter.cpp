#include "StdOStreamErrorIndenter.hpp"

using std::endl;
using std::string;
using std::ostream;

namespace redengine {
namespace error {

	StdOStreamErrorIndenter::StdOStreamErrorIndenter(ostream& stream) : stream(stream) {}

	StdOStreamErrorIndenter::StdOStreamErrorIndenter(const StdOStreamErrorIndenter& indenter)
			: ErrorIndenterBase8(indenter), stream(indenter.stream) {}

	void StdOStreamErrorIndenter::writeString(const string& data) {
		stream << data;
	}

	void StdOStreamErrorIndenter::endLine() {
		stream << endl;
	}

}}

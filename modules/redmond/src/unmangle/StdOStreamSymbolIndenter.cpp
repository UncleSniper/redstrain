#include "StdOStreamSymbolIndenter.hpp"

using std::endl;
using std::string;
using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	StdOStreamSymbolIndenter::StdOStreamSymbolIndenter(ostream& stream) : stream(stream) {}

	StdOStreamSymbolIndenter::StdOStreamSymbolIndenter(const StdOStreamSymbolIndenter& indenter)
			: SymbolSinkIndenterBase8(indenter), stream(indenter.stream) {}

	void StdOStreamSymbolIndenter::writeString(const string& data) {
		stream << data;
	}

	void StdOStreamSymbolIndenter::startNewLine() {
		stream << endl;
	}

}}}

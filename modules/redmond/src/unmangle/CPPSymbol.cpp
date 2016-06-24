#include <sstream>

#include "CPPSymbol.hpp"
#include "StdOStreamSymbolSink.hpp"
#include "StdOStreamSymbolIndenter.hpp"

using std::string;
using std::ostream;
using std::stringstream;

namespace redengine {
namespace redmond {
namespace unmangle {

	CPPSymbol::CPPSymbol() {}

	CPPSymbol::CPPSymbol(const CPPSymbol&) {}

	CPPSymbol::~CPPSymbol() {}

	void CPPSymbol::print(ostream& stream) const {
		StdOStreamSymbolIndenter indenter(stream);
		StdOStreamSymbolSink sink(stream, indenter);
		print(sink);
	}

	string CPPSymbol::toString() const {
		stringstream ss;
		print(ss);
		return ss.str();
	}

}}}

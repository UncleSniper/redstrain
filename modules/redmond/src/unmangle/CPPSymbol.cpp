#include <sstream>

#include "CPPSymbol.hpp"

using std::string;
using std::ostream;
using std::stringstream;

namespace redengine {
namespace redmond {
namespace unmangle {

	CPPSymbol::CPPSymbol() {}

	CPPSymbol::CPPSymbol(const CPPSymbol&) {}

	CPPSymbol::~CPPSymbol() {}

	void CPPSymbol::print(ostream& out) const {
		bool lastWasGreater = false;
		print(out, lastWasGreater);
	}

	string CPPSymbol::toString() const {
		stringstream ss;
		bool lastWasGreater = false;
		print(ss, lastWasGreater);
		return ss.str();
	}

}}}

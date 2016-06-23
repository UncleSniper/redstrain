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

	void CPPSymbol::print(ostream&) const {
		/*TODO
		bool lastWasGreater = false;
		print(out, lastWasGreater);
		*/
	}

	string CPPSymbol::toString() const {
		//TODO: short circuit this
		stringstream ss;
		print(ss);
		return ss.str();
	}

}}}

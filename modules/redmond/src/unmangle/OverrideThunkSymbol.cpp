#include "OverrideThunkSymbol.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	OverrideThunkSymbol::OverrideThunkSymbol(const CallOffset& callOffset, CPPSymbol* targetFunction)
			: callOffset(callOffset), targetFunction(targetFunction) {}

	OverrideThunkSymbol::OverrideThunkSymbol(const OverrideThunkSymbol& symbol)
			: SpecialSymbol(symbol), callOffset(symbol.callOffset),
			targetFunction(symbol.targetFunction->cloneSymbol()) {}

	OverrideThunkSymbol::~OverrideThunkSymbol() {
		delete targetFunction;
	}

	SpecialSymbol::SpecialSymbolType OverrideThunkSymbol::getSpecialSymbolType() const {
		return SST_OVERRIDE_THUNK;
	}

	CPPSymbol* OverrideThunkSymbol::cloneSymbol() const {
		return new OverrideThunkSymbol(*this);
	}

	void OverrideThunkSymbol::print(ostream& out, bool& lastWasGreater) const {
		out << "<override thunk for ";
		lastWasGreater = false;
		targetFunction->print(out, lastWasGreater);
		if(lastWasGreater)
			out << ' ';
		out << '>';
		lastWasGreater = true;
	}

}}}

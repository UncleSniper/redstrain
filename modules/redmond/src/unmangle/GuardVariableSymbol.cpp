#include "Name.hpp"
#include "GuardVariableSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	GuardVariableSymbol::GuardVariableSymbol(Name* objectName) : objectName(objectName) {}

	GuardVariableSymbol::GuardVariableSymbol(const GuardVariableSymbol& symbol)
			: SpecialSymbol(symbol), objectName(symbol.objectName->cloneName()) {}

	GuardVariableSymbol::~GuardVariableSymbol() {
		delete name;
	}

	SpecialSymbol::SpecialSymbolType GuardVariableSymbol::getSpecialSymbolType() const {
		return SST_GUARD_VARIABLE;
	}

	CPPSymbol* GuardVariableSymbol::cloneSymbol() const {
		return new GuardVariableSymbol(*this);
	}

}}}

#include "Name.hpp"
#include "SymbolSink.hpp"
#include "GuardVariableSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	GuardVariableSymbol::GuardVariableSymbol(Name* objectName) : objectName(objectName) {}

	GuardVariableSymbol::GuardVariableSymbol(const GuardVariableSymbol& symbol)
			: SpecialSymbol(symbol), objectName(symbol.objectName->cloneName()) {}

	GuardVariableSymbol::~GuardVariableSymbol() {
		delete objectName;
	}

	SpecialSymbol::SpecialSymbolType GuardVariableSymbol::getSpecialSymbolType() const {
		return SST_GUARD_VARIABLE;
	}

	CPPSymbol* GuardVariableSymbol::cloneSymbol() const {
		return new GuardVariableSymbol(*this);
	}

	void GuardVariableSymbol::print(SymbolSink& sink) const {
		sink.putGuardVariable(*this);
	}

}}}

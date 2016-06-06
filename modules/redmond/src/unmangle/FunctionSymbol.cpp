#include "Name.hpp"
#include "FunctionSymbol.hpp"
#include "BareFunctionType.hpp"
#include "../unmangle-utils.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	FunctionSymbol::FunctionSymbol(Name* name, BareFunctionType* type) : name(name), type(type) {}

	FunctionSymbol::FunctionSymbol(const FunctionSymbol& symbol) : CPPSymbol(symbol) {
		name = symbol.name->cloneName();
		UnmanglePtr<Name> deleteName(name);
		type = symbol.type->cloneBareFunctionType();
		deleteName.ptr = NULL;
	}

	FunctionSymbol::~FunctionSymbol() {
		delete name;
		delete type;
	}

	CPPSymbol::SymbolType FunctionSymbol::getSymbolType() const {
		return ST_FUNCTION;
	}

	CPPSymbol* FunctionSymbol::cloneSymbol() const {
		return new FunctionSymbol(*this);
	}

}}}

#include "Name.hpp"
#include "FunctionSymbol.hpp"
#include "BareFunctionType.hpp"
#include "../unmangle-utils.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	FunctionSymbol::FunctionSymbol(Name* name, BareFunctionType* type) : name(name), type(type) {}

	FunctionSymbol::FunctionSymbol(const FunctionSymbol& symbol) : CPPSymbol(symbol) {
		name = symbol.name->cloneName();
		UnmanglePtr<Name> deleteName(name);
		type = new BareFunctionType(*symbol.type);
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

	void FunctionSymbol::print(ostream& out, bool& lastWasGreater) const {
		bool hasReturn = ...; //TODO
		name->print(out, lastWasGreater);
		out << '(';
		BareFunctionType::TypeIterator ptbegin, ptend;
		if(hasReturn)
			type->getRestTypes(ptbegin, ptend);
		else
			type->getTypes(ptbegin, ptend);
		bool first = true;
		for(; ptbegin != ptend; ++ptbegin) {
			if(first)
				first = false;
			else
				out << ", ";
			lastWasGreater = false;
			(*ptbegin)->print(out, lastWasGreater);
		}
		out << ')';
		lastWasGreater = false;
	}

}}}

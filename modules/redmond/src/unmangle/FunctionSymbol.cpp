#include "Name.hpp"
#include "BuiltinType.hpp"
#include "FunctionSymbol.hpp"
#include "CVQualifiedType.hpp"
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
		bool hasReturn = name->namesTemplate() && !name->namesReturnless();
		CurrentTemplateArguments targuments;
		name->print(out, lastWasGreater, targuments, NULL);
		name->getCurrentTemplateArguments(targuments);
		out << '(';
		BareFunctionType::TypeIterator ptbegin, ptend;
		unsigned ptcount = type->getTypeCount();
		if(hasReturn) {
			type->getRestTypes(ptbegin, ptend);
			--ptcount;
		}
		else
			type->getTypes(ptbegin, ptend);
		bool first = true;
		for(; ptbegin != ptend; ++ptbegin) {
			if(first) {
				if(ptcount == 1u) {
					Type* pt = *ptbegin;
					if(pt->getTypeType() == Type::TT_BUILTIN
							&& static_cast<BuiltinType*>(pt)->getPrimitive() == BuiltinType::P_VOID)
						break;
				}
				first = false;
			}
			else
				out << ", ";
			lastWasGreater = false;
			(*ptbegin)->print(out, lastWasGreater, targuments);
		}
		out << ')';
		lastWasGreater = false;
		int qualifiers = name->getNameCVQualifiers();
		if(qualifiers & CVQualifiedType::CVQ_RESTRICT)
			out << " restrict";
		if(qualifiers & CVQualifiedType::CVQ_VOLATILE)
			out << " volatile";
		if(qualifiers & CVQualifiedType::CVQ_CONST)
			out << " const";
	}

}}}

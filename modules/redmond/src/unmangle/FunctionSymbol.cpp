#include "Name.hpp"
#include "SymbolSink.hpp"
#include "BuiltinType.hpp"
#include "FunctionSymbol.hpp"
#include "CVQualifiedType.hpp"
#include "BareFunctionType.hpp"
#include "../unmangle-utils.hpp"

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

	void FunctionSymbol::print(SymbolSink& sink) const {
		bool hasReturn = name->namesTemplate() && !name->namesReturnless();
		CurrentTemplateArguments targuments, realArguments;
		name->getCurrentTemplateArguments(realArguments);
		if(hasReturn && type->hasTypes()) {
			type->getFirstType()->print(sink, realArguments);
			sink.putSeparator(SymbolSink::SEP_RETURN_TYPE_FUNCTION);
		}
		name->print(sink, targuments, NULL);
		sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		BareFunctionType::TypeIterator ptbegin, ptend;
		unsigned ptcount = type->getTypeCount();
		if(hasReturn) {
			type->getRestTypes(ptbegin, ptend);
			--ptcount;
		}
		else
			type->getTypes(ptbegin, ptend);
		unsigned space = sink.getRemainingColumnCount();
		bool breakParams = false, first = true;
		if(space) {
			unsigned iwidth = 0u;
			BareFunctionType::TypeIterator sptbegin(ptbegin), sptend(ptend);
			for(; sptbegin != sptend; ++sptbegin) {
				if(first) {
					if(ptcount == 1u) {
						Type* pt = *sptbegin;
						if(pt->getTypeType() == Type::TT_BUILTIN
								&& static_cast<BuiltinType*>(pt)->getPrimitive() == BuiltinType::P_VOID)
							break;
					}
					first = false;
				}
				else
					iwidth += sink.getInlineWidthOf(SymbolSink::SEP_COMMA)
							+ sink.getInlineWidthOf(SymbolSink::SEP_AFTER_COMMA);
				iwidth += sink.getInlineWidthOf(**ptbegin, realArguments, NULL);
			}
			first = true;
			breakParams = iwidth > space;
		}
		for(; ptbegin != ptend; ++ptbegin) {
			if(first) {
				if(ptcount == 1u) {
					Type* pt = *ptbegin;
					if(pt->getTypeType() == Type::TT_BUILTIN
							&& static_cast<BuiltinType*>(pt)->getPrimitive() == BuiltinType::P_VOID)
						break;
				}
				first = false;
				if(breakParams)
					sink.startNewLine(1);
			}
			else {
				sink.putSeparator(SymbolSink::SEP_COMMA);
				if(breakParams)
					sink.startNewLine(0);
				else
					sink.putSeparator(SymbolSink::SEP_AFTER_COMMA);
			}
			(*ptbegin)->print(sink, realArguments);
		}
		if(breakParams && !first)
			sink.startNewLine(-1);
		sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
		int qualifiers = name->getNameCVQualifiers();
		if(qualifiers & CVQualifiedType::CVQ_RESTRICT) {
			sink.putSeparator(SymbolSink::SEP_BEFORE_CV);
			sink.putReserved(SymbolSink::RSV_RESTRICT);
		}
		if(qualifiers & CVQualifiedType::CVQ_VOLATILE) {
			sink.putSeparator(SymbolSink::SEP_BEFORE_CV);
			sink.putReserved(SymbolSink::RSV_VOLATILE);
		}
		if(qualifiers & CVQualifiedType::CVQ_CONST) {
			sink.putSeparator(SymbolSink::SEP_BEFORE_CV);
			sink.putReserved(SymbolSink::RSV_CONST);
		}
	}

}}}

#include "SymbolSink.hpp"
#include "BuiltinType.hpp"
#include "FunctionType.hpp"
#include "BareFunctionType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	FunctionType::FunctionType(bool externC, BareFunctionType* type) : externC(externC), type(type) {}

	FunctionType::FunctionType(const FunctionType& type)
			: Type(type), externC(type.externC), type(new BareFunctionType(*type.type)) {}

	FunctionType::~FunctionType() {
		delete type;
	}

	Type::TypeType FunctionType::getTypeType() const {
		return TT_FUNCTION;
	}

	bool FunctionType::inlinesEnclosingClassName() const {
		return true;
	}

	void FunctionType::print(SymbolSink& sink, const CurrentTemplateArguments& arguments,
			const Type* enclosingClass) const {
		Type* returnType = type->getFirstType();
		if(returnType)
			returnType->print(sink, arguments);
		else
			sink.putReplacementString(SymbolSink::RS_UNKNOWN_RETURN_TYPE);
		sink.putSeparator(SymbolSink::SEP_RETURN_TYPE_FUNCTION);
		sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		if(enclosingClass) {
			enclosingClass->print(sink, arguments, NULL);
			sink.putSeparator(SymbolSink::SEP_PAAMAYIM_NEKUDOTAYIM);
		}
		sink.putSeparator(SymbolSink::SEP_POINTER);
		sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
		sink.putSeparator(SymbolSink::SEP_LEFT_ROUND);
		unsigned ptcount = type->getTypeCount() - 1u;
		unsigned space = sink.getRemainingColumnCount();
		BareFunctionType::TypeIterator ptbegin, ptend;
		type->getRestTypes(ptbegin, ptend);
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
				iwidth += sink.getInlineWidthOf(**ptbegin, arguments, NULL);
			}
			iwidth += sink.getInlineWidthOf(SymbolSink::SEP_RIGHT_ROUND);
			first = true;
			breakParams = iwidth >= space;
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
			(*ptbegin)->print(sink, arguments);
		}
		if(breakParams && !first)
			sink.startNewLine(-1);
		sink.putSeparator(SymbolSink::SEP_RIGHT_ROUND);
	}

	Type* FunctionType::cloneType() const {
		return new FunctionType(*this);
	}

}}}

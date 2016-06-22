#include "BuiltinType.hpp"
#include "FunctionType.hpp"
#include "BareFunctionType.hpp"

using std::ostream;

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

	void FunctionType::print(ostream& out, bool& lastWasGreater, const CurrentTemplateArguments& arguments,
			const Type* enclosingClass) const {
		Type* returnType = type->getFirstType();
		if(returnType)
			returnType->print(out, lastWasGreater, arguments);
		else
			out << "<unknown return type>";
		out << " (";
		if(enclosingClass) {
			lastWasGreater = false;
			enclosingClass->print(out, lastWasGreater, arguments, NULL);
			out << "::";
		}
		out << "*)(";
		lastWasGreater = false;
		BareFunctionType::TypeIterator ptbegin, ptend;
		type->getRestTypes(ptbegin, ptend);
		bool first = true;
		for(; ptbegin != ptend; ++ptbegin) {
			if(first) {
				if(type->getTypeCount() == 2u) {
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
			(*ptbegin)->print(out, lastWasGreater, arguments);
		}
		out << ')';
		lastWasGreater = false;
	}

	Type* FunctionType::cloneType() const {
		return new FunctionType(*this);
	}

}}}

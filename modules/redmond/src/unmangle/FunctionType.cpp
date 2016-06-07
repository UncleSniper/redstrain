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

	void FunctionType::print(ostream& out, bool& lastWasGreater) const {
		Type* returnType = type->getFirstType();
		if(returnType)
			returnType->print(out, lastWasGreater);
		else
			out << "<unknown return type>";
		out << " (*)(";
		lastWasGreater = false;
		BareFunctionType::TypeIterator ptbegin, ptend;
		type->getRestTypes(ptbegin, ptend);
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

	Type* FunctionType::cloneType() const {
		return new FunctionType(*this);
	}

}}}

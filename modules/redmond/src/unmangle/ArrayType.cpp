#include "ArrayType.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	ArrayType::ArrayType(Type* elementType) : elementType(elementType) {}

	ArrayType::ArrayType(const ArrayType& type) : Type(type), elementType(type.elementType->cloneType()) {}

	ArrayType::~ArrayType() {
		delete elementType;
	}

	Type::TypeType ArrayType::getTypeType() const {
		return TT_ARRAY;
	}

	bool ArrayType::inlinesEnclosingClassName() const {
		return elementType->inlinesEnclosingClassName();
	}

	void ArrayType::print(ostream& out, bool& lastWasGreater, const CurrentTemplateArguments& arguments,
			const Type* enclosingClass) const {
		elementType->print(out, lastWasGreater, arguments, enclosingClass);
		out << '[';
		lastWasGreater = false;
		printDimension(out, arguments);
		out << ']';
		lastWasGreater = false;
	}

}}}

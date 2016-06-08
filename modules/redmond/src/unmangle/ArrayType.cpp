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

	void ArrayType::print(ostream& out, bool& lastWasGreater) const {
		elementType->print(out, lastWasGreater);
		out << '[';
		lastWasGreater = false;
		printDimension(out);
		out << ']';
		lastWasGreater = false;
	}

}}}

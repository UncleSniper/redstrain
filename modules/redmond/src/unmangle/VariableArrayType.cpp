#include "VariableArrayType.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	VariableArrayType::VariableArrayType(Type* elementType) : ArrayType(elementType) {}

	VariableArrayType::VariableArrayType(const VariableArrayType& type) : ArrayType(type) {}

	ArrayType::DimensionType VariableArrayType::getDimensionType() const {
		return DT_VARIABLE;
	}

	Type* VariableArrayType::cloneType() const {
		return new VariableArrayType(*this);
	}

	void VariableArrayType::printDimension(ostream&, const CurrentTemplateArguments&) const {}

}}}

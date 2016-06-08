#include "Expression.hpp"
#include "ComplexArrayType.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	ComplexArrayType::ComplexArrayType(Type* elementType, Expression* dimension)
			: ArrayType(elementType), dimension(dimension) {}

	ComplexArrayType::ComplexArrayType(const ComplexArrayType& type)
			: ArrayType(type), dimension(type.dimension->cloneType()) {}

	ComplexArrayType::~ComplexArrayType() {
		delete dimension;
	}

	ArrayType::DimensionType ComplexArrayType::getDimensionType() const {
		return DT_COMPLEX;
	}

	Type* ComplexArrayType::cloneType() const {
		return new ComplexArrayType(*this);
	}

	void ComplexArrayType::printDimension(ostream& out) const {
		dimension->print(out);
	}

}}}

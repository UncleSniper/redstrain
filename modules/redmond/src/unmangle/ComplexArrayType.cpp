#include "Expression.hpp"
#include "ComplexArrayType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	ComplexArrayType::ComplexArrayType(Type* elementType, Expression* dimension)
			: ArrayType(elementType), dimension(dimension) {}

	ComplexArrayType::ComplexArrayType(const ComplexArrayType& type)
			: ArrayType(type), dimension(type.dimension->cloneExpression()) {}

	ComplexArrayType::~ComplexArrayType() {
		delete dimension;
	}

	ArrayType::DimensionType ComplexArrayType::getDimensionType() const {
		return DT_COMPLEX;
	}

	Type* ComplexArrayType::cloneType() const {
		return new ComplexArrayType(*this);
	}

	void ComplexArrayType::printDimension(SymbolSink& sink, const CurrentTemplateArguments& arguments) const {
		dimension->print(sink, arguments);
	}

}}}

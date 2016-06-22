#include "StaticArrayType.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	StaticArrayType::StaticArrayType(Type* elementType, unsigned dimension)
			: ArrayType(elementType), dimension(dimension) {}

	StaticArrayType::StaticArrayType(const StaticArrayType& type) : ArrayType(type), dimension(type.dimension) {}

	ArrayType::DimensionType StaticArrayType::getDimensionType() const {
		return DT_STATIC;
	}

	Type* StaticArrayType::cloneType() const {
		return new StaticArrayType(*this);
	}

	void StaticArrayType::printDimension(ostream& out, const CurrentTemplateArguments&) const {
		out << dimension;
	}

}}}

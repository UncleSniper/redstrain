#include "SymbolSink.hpp"
#include "StaticArrayType.hpp"

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

	void StaticArrayType::printDimension(SymbolSink& sink, const CurrentTemplateArguments&) const {
		sink.putIntLiteral(static_cast<uint64_t>(dimension));
	}

}}}

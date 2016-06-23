#include "ArrayType.hpp"
#include "SymbolSink.hpp"

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

	void ArrayType::print(SymbolSink& sink, const CurrentTemplateArguments& arguments,
			const Type* enclosingClass) const {
		elementType->print(sink, arguments, enclosingClass);
		sink.putSeparator(SymbolSink::SEP_LEFT_SQUARE);
		printDimension(sink, arguments);
		sink.putSeparator(SymbolSink::SEP_RIGHT_SQUARE);
	}

}}}

#include "SymbolSink.hpp"
#include "CVQualifiedType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	CVQualifiedType::CVQualifiedType(int qualifiers, Type* type) : qualifiers(qualifiers), type(type) {}

	CVQualifiedType::CVQualifiedType(const CVQualifiedType& type)
			: Type(type), qualifiers(type.qualifiers), type(type.type->cloneType()) {}

	CVQualifiedType::~CVQualifiedType() {
		delete type;
	}

	Type::TypeType CVQualifiedType::getTypeType() const {
		return TT_CV_QUALIFIED;
	}

	bool CVQualifiedType::inlinesEnclosingClassName() const {
		return type->inlinesEnclosingClassName();
	}

	void CVQualifiedType::print(SymbolSink& sink, const CurrentTemplateArguments& arguments,
			const Type* enclosingClass) const {
		bool needsBlank;
		if(qualifiers & CVQualifiedType::CVQ_RESTRICT) {
			sink.putReserved(SymbolSink::RSV_RESTRICT);
			needsBlank = true;
		}
		else
			needsBlank = false;
		if(qualifiers & CVQualifiedType::CVQ_VOLATILE) {
			if(needsBlank)
				sink.putSeparator(SymbolSink::SEP_BEFORE_CV);
			sink.putReserved(SymbolSink::RSV_VOLATILE);
			needsBlank = true;
		}
		if(needsBlank)
			sink.putSeparator(SymbolSink::SEP_AFTER_CV);
		type->print(sink, arguments, enclosingClass);
		if(qualifiers & CVQualifiedType::CVQ_CONST) {
			sink.putSeparator(SymbolSink::SEP_BEFORE_CV);
			sink.putReserved(SymbolSink::RSV_CONST);
		}
	}

	Type* CVQualifiedType::cloneType() const {
		return new CVQualifiedType(*this);
	}

}}}

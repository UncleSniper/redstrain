#include "SymbolSink.hpp"
#include "ModifiedType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	ModifiedType::ModifiedType(Modifier modifier, Type* type) : modifier(modifier), type(type) {}

	ModifiedType::ModifiedType(const ModifiedType& type)
			: Type(type), modifier(type.modifier), type(type.type->cloneType()) {}

	ModifiedType::~ModifiedType() {
		delete type;
	}

	Type::TypeType ModifiedType::getTypeType() const {
		return TT_MODIFIED;
	}

	bool ModifiedType::inlinesEnclosingClassName() const {
		return type->inlinesEnclosingClassName();
	}

	void ModifiedType::print(SymbolSink& sink, const CurrentTemplateArguments& arguments,
			const Type* enclosingClass) const {
		type->print(sink, arguments, enclosingClass);
		switch(modifier) {
			case MOD_POINTER:
				if(type->getTypeType() != Type::TT_FUNCTION)
					sink.putSeparator(SymbolSink::SEP_POINTER);
				break;
			case MOD_REFERENCE:
				sink.putSeparator(SymbolSink::SEP_REFERENCE);
				break;
			case MOD_COMPLEX:
				sink.putSeparator(SymbolSink::SEP_BEFORE_COMPLEX);
				sink.putReserved(SymbolSink::RSV_COMPLEX);
				break;
			case MOD_IMAGINARY:
				sink.putSeparator(SymbolSink::SEP_BEFORE_COMPLEX);
				sink.putReserved(SymbolSink::RSV_IMAGINARY);
				break;
			default:
				sink.putReplacementString(SymbolSink::RS_UNKNOWN_TYPE_MODIFIER);
				break;
		}
	}

	Type* ModifiedType::cloneType() const {
		return new ModifiedType(*this);
	}

}}}

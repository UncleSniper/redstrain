#include "Name.hpp"
#include "EnumType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	EnumType::EnumType(Name* name) : name(name) {}

	EnumType::EnumType(const EnumType& type) : Type(type), name(type.name->cloneName()) {}

	EnumType::~EnumType() {
		delete name;
	}

	Type::TypeType EnumType::getTypeType() const {
		return TT_ENUM;
	}

	void EnumType::print(SymbolSink& sink, const CurrentTemplateArguments& arguments, const Type*) const {
		name->print(sink, arguments, NULL);
	}

	Type* EnumType::cloneType() const {
		return new EnumType(*this);
	}

}}}

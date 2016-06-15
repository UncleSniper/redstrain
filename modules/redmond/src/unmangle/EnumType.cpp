#include "Name.hpp"
#include "EnumType.hpp"

using std::ostream;

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

	void EnumType::print(ostream& out, bool& lastWasGreater) const {
		name->print(out, lastWasGreater, NULL);
	}

	Type* EnumType::cloneType() const {
		return new EnumType(*this);
	}

}}}

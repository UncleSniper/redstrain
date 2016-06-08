#include "ModifiedType.hpp"

using std::ostream;

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

	void ModifiedType::print(ostream& out, bool& lastWasGreater) const {
		type->print(out, lastWasGreater);
		lastWasGreater = false;
		switch(modifier) {
			case MOD_POINTER:
				out << '*';
				break;
			case MOD_REFERENCE:
				out << '&';
				break;
			case MOD_COMPLEX:
				out << " _Complex";
				break;
			case MOD_IMAGINARY:
				out << " _Imaginary";
				break;
			default:
				out << " <unknown type modifier>";
				lastWasGreater = true;
				break;
		}
	}

	Type* ModifiedType::cloneType() const {
		return new ModifiedType(*this);
	}

}}}

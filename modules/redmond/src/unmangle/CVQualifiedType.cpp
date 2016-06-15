#include "CVQualifiedType.hpp"

using std::ostream;

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

	void CVQualifiedType::print(ostream& out, bool& lastWasGreater) const {
		bool needsBlank;
		if(qualifiers & CVQualifiedType::CVQ_RESTRICT) {
			lastWasGreater = false;
			out << "restrict";
			needsBlank = true;
		}
		else
			needsBlank = false;
		if(qualifiers & CVQualifiedType::CVQ_VOLATILE) {
			lastWasGreater = false;
			if(needsBlank)
				out << ' ';
			out << "volatile";
			needsBlank = true;
		}
		if(needsBlank)
			out << ' ';
		type->print(out, lastWasGreater);
		if(qualifiers & CVQualifiedType::CVQ_CONST) {
			out << " const";
			lastWasGreater = false;
		}
	}

	Type* CVQualifiedType::cloneType() const {
		return new CVQualifiedType(*this);
	}

}}}

#include "PointerToMemberType.hpp"
#include "../unmangle-utils.hpp"

using std::ostream;

namespace redengine {
namespace redmond {
namespace unmangle {

	PointerToMemberType::PointerToMemberType(Type* classType, Type* memberType)
			: classType(classType), memberType(memberType) {}

	PointerToMemberType::PointerToMemberType(const PointerToMemberType& type)
			: Type(type), classType(type.classType->cloneType()) {
		UnmanglePtr<Type> ct(classType);
		memberType = type.memberType->cloneType();
		ct.ptr = NULL;
	}

	PointerToMemberType::~PointerToMemberType() {
		delete classType;
		delete memberType;
	}

	Type::TypeType PointerToMemberType::getTypeType() const {
		return TT_POINTER_TO_MEMBER;
	}

	void PointerToMemberType::print(ostream& out, bool& lastWasGreater, const CurrentTemplateArguments& arguments,
			const Type*) const {
		if(memberType->inlinesEnclosingClassName())
			memberType->print(out, lastWasGreater, arguments, classType);
		else {
			memberType->print(out, lastWasGreater, arguments, NULL);
			out << ' ';
			lastWasGreater = false;
			classType->print(out, lastWasGreater, arguments, NULL);
			out << "::*";
			lastWasGreater = false;
		}
	}

	Type* PointerToMemberType::cloneType() const {
		return new PointerToMemberType(*this);
	}

}}}

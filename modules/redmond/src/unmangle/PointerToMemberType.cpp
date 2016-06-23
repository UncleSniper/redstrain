#include "SymbolSink.hpp"
#include "PointerToMemberType.hpp"
#include "../unmangle-utils.hpp"

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

	void PointerToMemberType::print(SymbolSink& sink, const CurrentTemplateArguments& arguments, const Type*) const {
		if(memberType->inlinesEnclosingClassName())
			memberType->print(sink, arguments, classType);
		else {
			memberType->print(sink, arguments, NULL);
			sink.putSeparator(SymbolSink::SEP_TYPE_VARIABLE);
			classType->print(sink, arguments, NULL);
			sink.putSeparator(SymbolSink::SEP_PAAMAYIM_NEKUDOTAYIM);
			sink.putSeparator(SymbolSink::SEP_POINTER);
		}
	}

	Type* PointerToMemberType::cloneType() const {
		return new PointerToMemberType(*this);
	}

}}}

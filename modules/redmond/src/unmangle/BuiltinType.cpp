#include "SymbolSink.hpp"
#include "BuiltinType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	BuiltinType::BuiltinType(Primitive primitive) : primitive(primitive) {}

	BuiltinType::BuiltinType(const BuiltinType& type) : Type(type), primitive(type.primitive) {}

	Type::TypeType BuiltinType::getTypeType() const {
		return TT_BUILTIN;
	}

	void BuiltinType::print(SymbolSink& sink, const CurrentTemplateArguments&, const Type*) const {
		sink.putBuiltinType(*this);
	}

	Type* BuiltinType::cloneType() const {
		return new BuiltinType(*this);
	}

	const char* BuiltinType::getPrimitiveName(Primitive primitive) {
		switch(primitive) {
			case P_VOID:
				return "void";
			case P_WCHAR_T:
				return "wchar_t";
			case P_BOOL:
				return "bool";
			case P_CHAR:
				return "char";
			case P_SIGNED_CHAR:
				return "signed char";
			case P_UNSIGNED_CHAR:
				return "unsigned char";
			case P_SHORT:
				return "short";
			case P_UNSIGNED_SHORT:
				return "unsigned short";
			case P_INT:
				return "int";
			case P_UNSIGNED_INT:
				return "unsigned";
			case P_LONG:
				return "long";
			case P_UNSIGNED_LONG:
				return "unsigned long";
			case P_LONG_LONG:
				return "long long";
			case P_UNSIGNED_LONG_LONG:
				return "unsigned long long";
			case P_INT128:
				return "__int128";
			case P_UNSIGNED_INT128:
				return "unsigned __int128";
			case P_FLOAT:
				return "float";
			case P_DOUBLE:
				return "double";
			case P_LONG_DOUBLE:
				return "long double";
			case P_FLOAT128:
				return "__float128";
			case P_ELLIPSIS:
				return "...";
			default:
				return "<unknown builtin type>";
		}
	}

}}}

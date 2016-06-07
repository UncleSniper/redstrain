#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_BUILTINTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_BUILTINTYPE_HPP

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API BuiltinType : public Type {

	  public:
		enum Primitive {
			P_VOID,
			P_WCHAR_T,
			P_BOOL,
			P_CHAR,
			P_SIGNED_CHAR,
			P_UNSIGNED_CHAR,
			P_SHORT,
			P_UNSIGNED_SHORT,
			P_INT,
			P_UNSIGNED_INT,
			P_LONG,
			P_UNSIGNED_LONG,
			P_LONG_LONG,
			P_UNSIGNED_LONG_LONG,
			P_INT128,
			P_UNSIGNED_INT128,
			P_FLOAT,
			P_DOUBLE,
			P_LONG_DOUBLE,
			P_FLOAT128,
			P_ELLIPSIS
		};

	  private:
		Primitive primitive;

	  public:
		BuiltinType(Primitive);
		BuiltinType(const BuiltinType&);

		inline Primitive getPrimitive() const {
			return primitive;
		}

		virtual TypeType getTypeType() const;
		virtual void print(std::ostream&, bool&) const;
		virtual Type* cloneType() const;

		static const char* getPrimitiveName(Primitive);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_BUILTINTYPE_HPP */

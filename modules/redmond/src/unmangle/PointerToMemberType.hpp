#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_POINTERTOMEMBERTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_POINTERTOMEMBERTYPE_HPP

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API PointerToMemberType : public Type {

	  private:
		Type *classType, *memberType;

	  public:
		PointerToMemberType(Type*, Type*);
		PointerToMemberType(const PointerToMemberType&);
		virtual ~PointerToMemberType();

		virtual TypeType getTypeType() const;
		virtual void print(SymbolSink&, const CurrentTemplateArguments&, const Type*) const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_POINTERTOMEMBERTYPE_HPP */

#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_MODIFIEDTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_MODIFIEDTYPE_HPP

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API ModifiedType : public Type {

	  public:
		enum Modifier {
			MOD_POINTER,
			MOD_REFERENCE,
			MOD_COMPLEX,
			MOD_IMAGINARY
		};

	  private:
		Modifier modifier;
		Type* type;

	  public:
		ModifiedType(Modifier, Type*);
		ModifiedType(const ModifiedType&);
		virtual ~ModifiedType();

		virtual TypeType getTypeType() const;
		virtual bool inlinesEnclosingClassName() const;
		virtual void print(SymbolSink&, const CurrentTemplateArguments&, const Type*) const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_MODIFIEDTYPE_HPP */

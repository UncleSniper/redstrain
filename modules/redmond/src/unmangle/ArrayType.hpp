#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_ARRAYTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_ARRAYTYPE_HPP

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API ArrayType : public Type {

	  public:
		enum DimensionType {
			DT_STATIC,
			DT_COMPLEX,
			DT_VARIABLE
		};

	  private:
		Type* elementType;

	  protected:
		virtual void printDimension(SymbolSink&, const CurrentTemplateArguments&) const = 0;

	  public:
		ArrayType(Type*);
		ArrayType(const ArrayType&);
		virtual ~ArrayType();

		virtual TypeType getTypeType() const;
		virtual bool inlinesEnclosingClassName() const;
		virtual void print(SymbolSink&, const CurrentTemplateArguments&, const Type*) const;
		virtual DimensionType getDimensionType() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_ARRAYTYPE_HPP */

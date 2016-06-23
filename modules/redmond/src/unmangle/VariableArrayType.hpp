#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_VARIABLEARRAYTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_VARIABLEARRAYTYPE_HPP

#include "ArrayType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API VariableArrayType : public ArrayType {

	  protected:
		virtual void printDimension(SymbolSink&, const CurrentTemplateArguments&) const;

	  public:
		VariableArrayType(Type*);
		VariableArrayType(const VariableArrayType&);

		virtual DimensionType getDimensionType() const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_VARIABLEARRAYTYPE_HPP */

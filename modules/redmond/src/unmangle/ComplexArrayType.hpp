#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_COMPLEXARRAYTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_COMPLEXARRAYTYPE_HPP

#include "ArrayType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Expression;

	class REDSTRAIN_REDMOND_API ComplexArrayType : public ArrayType {

	  private:
		Expression* dimension;

	  protected:
		virtual void printDimension(std::ostream&) const;

	  public:
		ComplexArrayType(Type*, Expression*);
		ComplexArrayType(const ComplexArrayType&);
		virtual ~ComplexArrayType();

		virtual DimensionType getDimensionType() const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_COMPLEXARRAYTYPE_HPP */

#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_STATICARRAYTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_STATICARRAYTYPE_HPP

#include "ArrayType.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API StaticArrayType : public ArrayType {

	  private:
		unsigned dimension;

	  protected:
		virtual void printDimension(std::ostream&, const CurrentTemplateArguments&) const;

	  public:
		StaticArrayType(Type*, unsigned);
		StaticArrayType(const StaticArrayType&);

		virtual DimensionType getDimensionType() const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_STATICARRAYTYPE_HPP */

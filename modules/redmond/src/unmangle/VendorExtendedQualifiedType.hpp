#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_VENDOREXTENDEDQUALIFIEDTYPE_CPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_VENDOREXTENDEDQUALIFIEDTYPE_CPP

#include <string>

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API VendorExtendedQualifiedType : public Type {

	  private:
		std::string name;
		Type* type;

	  public:
		VendorExtendedQualifiedType(const std::string&, Type*);
		VendorExtendedQualifiedType(const VendorExtendedQualifiedType&);
		virtual ~VendorExtendedQualifiedType();

		virtual TypeType getTypeType() const;
		virtual bool inlinesEnclosingClassName() const;
		virtual void print(std::ostream&, bool&, const CurrentTemplateArguments&, const Type*) const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_VENDOREXTENDEDQUALIFIEDTYPE_CPP */

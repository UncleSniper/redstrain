#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_VENDOREXTENDEDTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_VENDOREXTENDEDTYPE_HPP

#include <string>

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API VendorExtendedType : public Type {

	  private:
		std::string name;

	  public:
		VendorExtendedType(const std::string&);
		VendorExtendedType(const VendorExtendedType&);
		virtual ~VendorExtendedType();

		inline const std::string& getName() const {
			return name;
		}

		virtual TypeType getTypeType() const;
		virtual void print(std::ostream&, bool&, const CurrentTemplateArguments&, const Type*) const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_VENDOREXTENDEDTYPE_HPP */

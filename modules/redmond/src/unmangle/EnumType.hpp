#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_ENUMTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_ENUMTYPE_HPP

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Name;

	class REDSTRAIN_REDMOND_API EnumType : public Type {

	  private:
		Name* name;

	  public:
		EnumType(Name*);
		EnumType(const EnumType&);
		virtual ~EnumType();

		inline Name& getName() const {
			return *name;
		}

		virtual TypeType getTypeType() const;
		virtual void print(std::ostream&, bool&) const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_ENUMTYPE_HPP */

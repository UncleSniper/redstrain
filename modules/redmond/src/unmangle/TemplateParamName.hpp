#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEPARAMNAME_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEPARAMNAME_HPP

#include "UnqualifiedName.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API TemplateParamName : public UnqualifiedName {

	  private:
		unsigned parameter;

	  public:
		TemplateParamName(unsigned);
		TemplateParamName(const TemplateParamName&);

		virtual NameType getNameType() const;
		virtual void print(std::ostream&, bool&, const CurrentTemplateArguments&, const std::string*) const;
		virtual Name* cloneName() const;
		virtual bool namesTemplate() const;
		virtual bool namesReturnless() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEPARAMNAME_HPP */

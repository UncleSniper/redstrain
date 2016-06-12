#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEARGUMENT_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEARGUMENT_HPP

#include <iostream>

#include "../api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API TemplateArgument {

	  public:
		enum ArgumentType {
			AT_TYPE,
			AT_EXPRESSION
		};

	  public:
		TemplateArgument();
		TemplateArgument(const TemplateArgument&);
		virtual ~TemplateArgument();

		virtual ArgumentType getArgumentType() const = 0;
		virtual void print(std::ostream&, bool&) const = 0;
		virtual TemplateArgument* cloneTemplateArgument() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEARGUMENT_HPP */

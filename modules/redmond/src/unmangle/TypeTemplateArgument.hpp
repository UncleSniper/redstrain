#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_TYPETEMPLATEARGUMENT_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_TYPETEMPLATEARGUMENT_HPP

#include "TemplateArgument.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Type;

	class REDSTRAIN_REDMOND_API TypeTemplateArgument : public TemplateArgument {

	  private:
		Type* type;

	  public:
		TypeTemplateArgument(Type*);
		TypeTemplateArgument(const TypeTemplateArgument&);
		virtual ~TypeTemplateArgument();

		inline Type& getType() const {
			return *type;
		}

		virtual ArgumentType getArgumentType() const;
		virtual void print(SymbolSink&, const CurrentTemplateArguments&) const;
		virtual TemplateArgument* cloneTemplateArgument() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_TYPETEMPLATEARGUMENT_HPP */

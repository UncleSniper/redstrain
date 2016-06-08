#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATETEMPLATEPARAMTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATETEMPLATEPARAMTYPE_HPP

#include <list>

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class TemplateArgument;

	class REDSTRAIN_REDMOND_API TemplateTemplateParamType : public Type {

	  private:
		typedef std::list<TemplateArgument*> Arguments;

	  public:
		typedef Arguments::const_iterator ArgumentIterator;

	  private:
		unsigned parameter;
		Arguments arguments;

	  public:
		TemplateTemplateParamType(unsigned);
		TemplateTemplateParamType(const TemplateTemplateParamType&);
		virtual ~TemplateTemplateParamType();

		unsigned getArgumentCount() const;
		void getArguments(ArgumentIterator&, ArgumentIterator&) const;
		void addArgument(TemplateArgument&);

		virtual TypeType getTypeType() const;
		virtual void print(std::ostream&, bool&) const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATETEMPLATEPARAMTYPE_HPP */

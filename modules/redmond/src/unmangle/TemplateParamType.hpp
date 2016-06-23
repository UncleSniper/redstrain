#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEPARAMTYPE_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEPARAMTYPE_HPP

#include "Type.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API TemplateParamType : public Type {

	  private:
		unsigned parameter;

	  public:
		TemplateParamType(unsigned);
		TemplateParamType(const TemplateParamType&);

		virtual TypeType getTypeType() const;
		virtual void print(SymbolSink&, const CurrentTemplateArguments&, const Type*) const;
		virtual Type* cloneType() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_TEMPLATEPARAMTYPE_HPP */

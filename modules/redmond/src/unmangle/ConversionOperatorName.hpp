#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_CONVERSIONOPERATORNAME_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_CONVERSIONOPERATORNAME_HPP

#include "UnqualifiedName.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Type;

	class REDSTRAIN_REDMOND_API ConversionOperatorName : public UnqualifiedName {

	  private:
		Type* targetType;

	  public:
		ConversionOperatorName(Type*);
		ConversionOperatorName(const ConversionOperatorName&);
		virtual ~ConversionOperatorName();

		virtual NameType getNameType() const;
		virtual void print(SymbolSink&, const CurrentTemplateArguments&, const std::string*) const;
		virtual Name* cloneName() const;
		virtual bool namesTemplate() const;
		virtual bool namesReturnless() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_CONVERSIONOPERATORNAME_HPP */

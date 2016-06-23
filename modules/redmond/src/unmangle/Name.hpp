#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_NAME_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_NAME_HPP

#include <string>

#include "types.hpp"
#include "../api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class SymbolSink;

	class REDSTRAIN_REDMOND_API Name {

	  public:
		enum NameType {
			NT_OPERATOR,
			NT_CONVERSION_OPERATOR,
			NT_CTOR_DTOR,
			NT_SOURCE,
			NT_NESTED,
			NT_LOCAL,
			NT_TEMPLATE_PARAM
		};

	  public:
		Name();
		Name(const Name&);
		virtual ~Name();

		virtual NameType getNameType() const = 0;
		virtual int getNameCVQualifiers() const;
		virtual void getCurrentTemplateArguments(CurrentTemplateArguments&) const;
		virtual void print(SymbolSink&, const CurrentTemplateArguments&, const std::string*) const = 0;
		virtual Name* cloneName() const = 0;
		virtual bool namesTemplate() const = 0;
		virtual bool namesReturnless() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_NAME_HPP */

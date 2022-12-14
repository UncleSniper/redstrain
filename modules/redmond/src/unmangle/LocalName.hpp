#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_LOCALNAME_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_LOCALNAME_HPP

#include "Name.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class CPPSymbol;

	class REDSTRAIN_REDMOND_API LocalName : public Name {

	  private:
		CPPSymbol* function;
		Name* name;
		unsigned discriminator;

	  public:
		LocalName(CPPSymbol*, Name*, unsigned);
		LocalName(const LocalName&);
		virtual ~LocalName();

		virtual NameType getNameType() const;
		virtual void print(SymbolSink&, const CurrentTemplateArguments&, const std::string*) const;
		virtual Name* cloneName() const;
		virtual bool namesTemplate() const;
		virtual bool namesReturnless() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_LOCALNAME_HPP */

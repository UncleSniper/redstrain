#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_GUARDVARIABLESYMBOL_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_GUARDVARIABLESYMBOL_HPP

#include "SpecialSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Name;

	class REDSTRAIN_REDMOND_API GuardVariableSymbol : public SpecialSymbol {

	  private:
		Name* objectName;

	  protected:
		virtual void print(std::ostream&, bool&) const;

	  public:
		GuardVariableSymbol(Name*);
		GuardVariableSymbol(const GuardVariableSymbol&);
		virtual ~GuardVariableSymbol();

		virtual SpecialSymbolType getSpecialSymbolType() const;
		virtual CPPSymbol* cloneSymbol() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_GUARDVARIABLESYMBOL_HPP */

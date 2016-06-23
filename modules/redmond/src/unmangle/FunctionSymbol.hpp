#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_FUNCTIONSYMBOL_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_FUNCTIONSYMBOL_HPP

#include "CPPSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Name;
	class BareFunctionType;

	class REDSTRAIN_REDMOND_API FunctionSymbol : public CPPSymbol {

	  private:
		Name* name;
		BareFunctionType* type;

	  public:
		FunctionSymbol(Name*, BareFunctionType*);
		FunctionSymbol(const FunctionSymbol&);
		virtual ~FunctionSymbol();

		virtual SymbolType getSymbolType() const;
		virtual void print(SymbolSink&) const;
		virtual CPPSymbol* cloneSymbol() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_FUNCTIONSYMBOL_HPP */

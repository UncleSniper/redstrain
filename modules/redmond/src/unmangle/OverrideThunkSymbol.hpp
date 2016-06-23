#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_OVERRIDETHUNKSYMBOL_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_OVERRIDETHUNKSYMBOL_HPP

#include "CallOffset.hpp"
#include "SpecialSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API OverrideThunkSymbol : public SpecialSymbol {

	  private:
		CallOffset callOffset;
		CPPSymbol* targetFunction;

	  public:
		OverrideThunkSymbol(const CallOffset&, CPPSymbol*);
		OverrideThunkSymbol(const OverrideThunkSymbol&);
		virtual ~OverrideThunkSymbol();

		virtual SpecialSymbolType getSpecialSymbolType() const;
		virtual void print(SymbolSink&) const;
		virtual CPPSymbol* cloneSymbol() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_OVERRIDETHUNKSYMBOL_HPP */

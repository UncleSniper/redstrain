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

	  protected:
		virtual void print(std::ostream&, bool&) const;

	  public:
		OverrideThunkSymbol(const CallOffset&, CPPSymbol*);
		OverrideThunkSymbol(const OverrideThunkSymbol&);
		virtual ~OverrideThunkSymbol();

		virtual SpecialSymbolType getSpecialSymbolType() const;
		virtual CPPSymbol* cloneSymbol() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_OVERRIDETHUNKSYMBOL_HPP */

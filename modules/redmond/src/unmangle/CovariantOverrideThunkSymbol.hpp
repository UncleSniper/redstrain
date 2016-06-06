#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_COVARIANTOVERRIDETHUNKSYMBOL_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_COVARIANTOVERRIDETHUNKSYMBOL_HPP

#include "CallOffset.hpp"
#include "SpecialSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API CovariantOverrideThunkSymbol : public SpecialSymbol {

	  private:
		CallOffset thisAdjustment, resultAdjustment;
		CPPSymbol* targetFunction;

	  public:
		CovariantOverrideThunkSymbol(const CallOffset&, const CallOffset&, CPPSymbol*);
		CovariantOverrideThunkSymbol(const CovariantOverrideThunkSymbol&);
		virtual ~CovariantOverrideThunkSymbol();

		virtual SpecialSymbolType getSpecialSymbolType() const;
		virtual CPPSymbol* cloneSymbol() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_COVARIANTOVERRIDETHUNKSYMBOL_HPP */

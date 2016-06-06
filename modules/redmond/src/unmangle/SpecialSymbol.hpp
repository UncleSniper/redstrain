#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_SPECIALSYMBOL_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_SPECIALSYMBOL_HPP

#include "CPPSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API SpecialSymbol : public CPPSymbol {

	  public:
		enum SpecialSymbolType {
			SST_TABLE,
			SST_GUARD_VARIABLE,
			SST_OVERRIDE_THUNK,
			SST_COVARIANT_OVERRIDE_THUNK
		};

	  public:
		SpecialSymbol();
		SpecialSymbol(const SpecialSymbol&);

		virtual SymbolType getSymbolType() const;
		virtual SpecialSymbolType getSpecialSymbolType() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_SPECIALSYMBOL_HPP */

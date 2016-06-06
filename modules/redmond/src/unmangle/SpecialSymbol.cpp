#include "SpecialSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	SpecialSymbol::SpecialSymbol() {}

	SpecialSymbol::SpecialSymbol(const SpecialSymbol& symbol) : CPPSymbol(symbol) {}

	CPPSymbol::SymbolType SpecialSymbol::getSymbolType() const {
		return ST_SPECIAL;
	}

}}}

#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_DATASYMBOL_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_DATASYMBOL_HPP

#include "CPPSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Name;

	class REDSTRAIN_REDMOND_API DataSymbol : public CPPSymbol {

	  private:
		Name* name;

	  public:
		DataSymbol(Name*);
		DataSymbol(const DataSymbol&);
		virtual ~DataSymbol();

		virtual SymbolType getSymbolType() const;
		virtual void print(std::ostream&, bool&) const;
		virtual CPPSymbol* cloneSymbol() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_DATASYMBOL_HPP */

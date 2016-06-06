#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_CPPSYMBOL_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_CPPSYMBOL_HPP

#include <string>
#include <iostream>

#include "../api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API CPPSymbol {

	  public:
		enum SymbolType {
			ST_FUNCTION,
			ST_DATA,
			ST_SPECIAL
		};

	  protected:
		virtual void print(std::ostream&, bool&) const = 0;

	  public:
		CPPSymbol();
		CPPSymbol(const CPPSymbol&);
		virtual ~CPPSymbol();

		void print(std::ostream&) const;
		std::string toString() const;

		virtual SymbolType getSymbolType() const = 0;
		virtual CPPSymbol* cloneSymbol() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_CPPSYMBOL_HPP */

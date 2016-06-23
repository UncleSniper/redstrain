#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_CPPSYMBOL_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_CPPSYMBOL_HPP

#include <string>
#include <iostream>

#include "../api.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class SymbolSink;

	class REDSTRAIN_REDMOND_API CPPSymbol {

	  public:
		enum SymbolType {
			ST_FUNCTION,
			ST_DATA,
			ST_SPECIAL
		};

	  public:
		CPPSymbol();
		CPPSymbol(const CPPSymbol&);
		virtual ~CPPSymbol();

		void print(std::ostream&) const;
		std::string toString() const;

		virtual SymbolType getSymbolType() const = 0;
		virtual void print(SymbolSink&) const = 0;
		virtual CPPSymbol* cloneSymbol() const = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_CPPSYMBOL_HPP */

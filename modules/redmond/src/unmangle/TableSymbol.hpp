#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_TABLESYMBOL_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_TABLESYMBOL_HPP

#include "SpecialSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class Type;

	class REDSTRAIN_REDMOND_API TableSymbol : public SpecialSymbol {

	  public:
		enum TableSymbolType {
			TST_VIRTUAL_TABLE,
			TST_VTT_STRUCTURE,
			TST_TYPEINFO_STRUCTURE,
			TST_TYPEINFO_NAME
		};

	  private:
		TableSymbolType tableType;
		Type* targetType;

	  public:
		TableSymbol(TableSymbolType, Type*);
		TableSymbol(const TableSymbol&);
		virtual ~TableSymbol();

		inline TableSymbolType getTableType() const {
			return tableType;
		}

		inline Type& getTargetType() const {
			return *targetType;
		}

		virtual SpecialSymbolType getSpecialSymbolType() const;
		virtual void print(SymbolSink&) const;
		virtual CPPSymbol* cloneSymbol() const;

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_TABLESYMBOL_HPP */

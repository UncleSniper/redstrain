#include "Type.hpp"
#include "SymbolSink.hpp"
#include "TableSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	TableSymbol::TableSymbol(TableSymbolType tableType, Type* targetType)
			: tableType(tableType), targetType(targetType) {}

	TableSymbol::TableSymbol(const TableSymbol& symbol)
			: SpecialSymbol(symbol), tableType(symbol.tableType), targetType(symbol.targetType->cloneType()) {}

	TableSymbol::~TableSymbol() {
		delete targetType;
	}

	SpecialSymbol::SpecialSymbolType TableSymbol::getSpecialSymbolType() const {
		return SST_TABLE;
	}

	CPPSymbol* TableSymbol::cloneSymbol() const {
		return new TableSymbol(*this);
	}

	void TableSymbol::print(SymbolSink& sink) const {
		sink.putTable(*this);
	}

}}}

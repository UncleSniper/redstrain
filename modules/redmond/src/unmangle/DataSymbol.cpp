#include "Name.hpp"
#include "DataSymbol.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	DataSymbol::DataSymbol(Name* name) : name(name) {}

	DataSymbol::DataSymbol(const DataSymbol& symbol) : CPPSymbol(symbol), name(symbol.name->cloneName()) {}

	DataSymbol::~DataSymbol() {
		delete name;
	}

	CPPSymbol::SymbolType DataSymbol::getSymbolType() const {
		return ST_DATA;
	}

	CPPSymbol* DataSymbol::cloneSymbol() const {
		return new DataSymbol(*this);
	}

}}}

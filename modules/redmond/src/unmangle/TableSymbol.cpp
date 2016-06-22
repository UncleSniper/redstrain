#include "Type.hpp"
#include "TableSymbol.hpp"

using std::ostream;

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

	void TableSymbol::print(ostream& out, bool& lastWasGreater) const {
		switch(tableType) {
			case TST_VIRTUAL_TABLE:
				out << "<virtual table for ";
				break;
			case TST_VTT_STRUCTURE:
				out << "<VTT structure for ";
				break;
			case TST_TYPEINFO_STRUCTURE:
				out << "<typeinfo structure for ";
				break;
			case TST_TYPEINFO_NAME:
				out << "<typeinfo name for ";
				break;
			default:
				out << "<table object of unknown type, for type ";
				break;
		}
		lastWasGreater = false;
		CurrentTemplateArguments targuments;
		targetType->print(out, lastWasGreater, targuments);
		if(lastWasGreater)
			out << ' ';
		out << '>';
		lastWasGreater = true;
	}

}}}

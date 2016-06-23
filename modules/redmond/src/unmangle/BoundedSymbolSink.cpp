#include "BoundedSymbolSink.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	BoundedSymbolSink::BoundedSymbolSink(unsigned columnCount) : columnCount(columnCount), currentColumn(0u) {}

	BoundedSymbolSink::BoundedSymbolSink(const BoundedSymbolSink& sink)
			: SymbolSink(sink), columnCount(sink.columnCount), currentColumn(sink.currentColumn) {}

	void BoundedSymbolSink::advanceCurrentColumn(unsigned columnDelta) {
		currentColumn += columnDelta;
	}

	void BoundedSymbolSink::resetCurrentColumn() {
		currentColumn = 0u;
	}

	unsigned BoundedSymbolSink::getRemainingColumnCount() {
		return currentColumn > columnCount ? 0u : columnCount - currentColumn;
	}

}}}

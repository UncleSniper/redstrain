#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "TerminalBinding.hpp"
#include "TerminalTooDumbError.hpp"
#include "BindingTerminalCanvas.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::util::MemorySize;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;

namespace redengine {
namespace damnation {

	BindingTerminalCanvas::BindingTerminalCanvas(TerminalBinding& binding)
			: binding(binding), currentSize(binding.getSize()), sizeChangePending(false), cursorRow(0u),
			cursorColumn(0u), carriageReturnStrategy(BindingTerminalCanvas::determineCarriageReturnStrategy(binding)),
			clearScreenStrategy(BindingTerminalCanvas::determineClearScreenStrategy(binding)),
			homeRowStrategy(BindingTerminalCanvas::determineHomeRowStrategy(binding)),
			moveToStrategy(BindingTerminalCanvas::determineMoveToStrategy(binding)),
			rowToStrategy(BindingTerminalCanvas::determineRowToStrategy(binding)),
			columnToStrategy(BindingTerminalCanvas::determineColumnToStrategy(binding)),
			cursorDownStrategy(BindingTerminalCanvas::determineCursorDownStrategy(binding)),
			cursorDownByStrategy(BindingTerminalCanvas::determineCursorDownByStrategy(binding)),
			cursorUpStrategy(BindingTerminalCanvas::determineCursorUpStrategy(binding)),
			cursorUpByStrategy(BindingTerminalCanvas::determineCursorUpByStrategy(binding)),
			cursorLeftStrategy(BindingTerminalCanvas::determineCursorLeftStrategy(binding)),
			cursorLeftByStrategy(BindingTerminalCanvas::determineCursorLeftByStrategy(binding)),
			cursorRightStrategy(BindingTerminalCanvas::determineCursorRightStrategy(binding)),
			cursorRightByStrategy(BindingTerminalCanvas::determineCursorRightByStrategy(binding)) {
		if(homeRowStrategy == HRS_IMPOSSIBLE) {
			if(clearScreenStrategy != CSS_CLEAR_SCREEN)
				throw TerminalTooDumbError();
		}
		binding.startTerminalUse(true);
		binding.setInputMode(TerminalBinding::IM_RAW);
		binding.resetTextAttributes();
		binding.clearScreen() || binding.moveTo(0u, 0u) || (binding.rowTo(0u), binding.columnTo(0u));
	}

	BindingTerminalCanvas::BindingTerminalCanvas(const BindingTerminalCanvas& canvas)
			: TerminalCanvas(canvas), binding(canvas.binding), currentSize(canvas.currentSize),
			sizeChangePending(canvas.sizeChangePending), cursorRow(canvas.cursorRow),
			cursorColumn(canvas.cursorColumn), carriageReturnStrategy(canvas.carriageReturnStrategy),
			clearScreenStrategy(canvas.clearScreenStrategy), homeRowStrategy(canvas.homeRowStrategy),
			moveToStrategy(canvas.moveToStrategy), rowToStrategy(canvas.rowToStrategy),
			columnToStrategy(canvas.columnToStrategy), cursorDownStrategy(canvas.cursorDownStrategy),
			cursorDownByStrategy(canvas.cursorDownByStrategy), cursorUpStrategy(canvas.cursorUpStrategy),
			cursorUpByStrategy(canvas.cursorUpByStrategy), cursorLeftStrategy(canvas.cursorLeftStrategy),
			cursorLeftByStrategy(canvas.cursorLeftByStrategy), cursorRightStrategy(canvas.cursorRightStrategy),
			cursorRightByStrategy(canvas.cursorRightByStrategy) {}

#define mapStrategy(operation, strategy) \
	if(binding.supportsOperation(TerminalBinding::operation)) \
		return strategy;
#define mapStrategy2(operationA, operationB, strategy) \
	if(binding.supportsOperation(TerminalBinding::operationA) \
			&& binding.supportsOperation(TerminalBinding::operationB)) \
		return strategy;

	BindingTerminalCanvas::CarriageReturnStrategy
	BindingTerminalCanvas::determineCarriageReturnStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CARRIAGE_RETURN, CRS_CARRIAGE_RETURN)
		mapStrategy(OO_COLUMN_TO, CRS_COLUMN_TO)
		mapStrategy(OO_COLUMN_BY, CRS_COLUMN_BY)
		mapStrategy(OO_MOVE_TO, CRS_MOVE_TO)
		mapStrategy(OO_MOVE_BY, CRS_MOVE_BY)
		mapStrategy(OO_CURSOR_LEFT_BY, CRS_CURSOR_LEFT_BY)
		mapStrategy(OO_CURSOR_LEFT, CRS_CURSOR_LEFT)
		throw TerminalTooDumbError();
	}

	BindingTerminalCanvas::ClearScreenStrategy
	BindingTerminalCanvas::determineClearScreenStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CLEAR_SCREEN, CSS_CLEAR_SCREEN)
		mapStrategy(OO_ROW_TO, CSS_ROW_TO)
		mapStrategy(OO_ROW_BY, CSS_ROW_BY)
		mapStrategy(OO_MOVE_TO, CSS_MOVE_TO)
		mapStrategy(OO_MOVE_BY, CSS_MOVE_BY)
		mapStrategy(OO_CURSOR_DOWN_BY, CSS_CURSOR_DOWN_BY)
		return CSS_NEW_LINE;
	}

	BindingTerminalCanvas::HomeRowStrategy
	BindingTerminalCanvas::determineHomeRowStrategy(TerminalBinding& binding) {
		mapStrategy(OO_ROW_TO, HRS_ROW_TO)
		mapStrategy(OO_ROW_BY, HRS_ROW_BY)
		mapStrategy(OO_MOVE_TO, HRS_MOVE_TO)
		mapStrategy(OO_MOVE_BY, HRS_MOVE_BY)
		mapStrategy(OO_CURSOR_UP_BY, HRS_CURSOR_UP_BY)
		mapStrategy(OO_CURSOR_UP, HRS_CURSOR_UP)
		return HRS_IMPOSSIBLE;
	}

	BindingTerminalCanvas::MoveToStrategy
	BindingTerminalCanvas::determineMoveToStrategy(TerminalBinding& binding) {
		mapStrategy(OO_MOVE_TO, MTS_MOVE_TO)
		mapStrategy(OO_MOVE_BY, MTS_MOVE_BY)
		return MTS_SPLIT;
	}

	BindingTerminalCanvas::RowToStrategy
	BindingTerminalCanvas::determineRowToStrategy(TerminalBinding& binding) {
		mapStrategy(OO_ROW_TO, RTS_ROW_TO)
		mapStrategy(OO_ROW_BY, RTS_ROW_BY)
		mapStrategy(OO_MOVE_TO, RTS_MOVE_TO)
		mapStrategy(OO_MOVE_BY, RTS_MOVE_BY)
		mapStrategy2(OO_CURSOR_UP_BY, OO_CURSOR_DOWN_BY, RTS_CURSOR_UP_BY_DOWN_BY)
		mapStrategy2(OO_CURSOR_UP_BY, OO_CURSOR_DOWN, RTS_CURSOR_UP_BY_DOWN)
		mapStrategy2(OO_CURSOR_UP, OO_CURSOR_DOWN_BY, RTS_CURSOR_UP_DOWN_BY)
		mapStrategy2(OO_CURSOR_UP_BY, OO_COLUMN_TO, RTS_CURSOR_UP_BY_NEW_LINE_COLUMN_TO)
		mapStrategy2(OO_CURSOR_UP_BY, OO_COLUMN_BY, RTS_CURSOR_UP_BY_NEW_LINE_COLUMN_BY)
		mapStrategy2(OO_CURSOR_UP_BY, OO_CURSOR_RIGHT_BY, RTS_CURSOR_UP_BY_NEW_LINE_CURSOR_RIGHT_BY)
		mapStrategy2(OO_CURSOR_UP_BY, OO_CURSOR_RIGHT, RTS_CURSOR_UP_BY_NEW_LINE_CURSOR_RIGHT)
		mapStrategy2(OO_CURSOR_UP, OO_CURSOR_DOWN, RTS_CURSOR_UP_DOWN)
		mapStrategy2(OO_CURSOR_UP, OO_COLUMN_TO, RTS_CURSOR_UP_NEW_LINE_COLUMN_TO)
		mapStrategy2(OO_CURSOR_UP, OO_COLUMN_BY, RTS_CURSOR_UP_NEW_LINE_COLUMN_BY)
		mapStrategy2(OO_CURSOR_UP, OO_CURSOR_RIGHT_BY, RTS_CURSOR_UP_NEW_LINE_CURSOR_RIGHT_BY)
		mapStrategy2(OO_CURSOR_UP, OO_CURSOR_RIGHT, RTS_CURSOR_UP_NEW_LINE_CURSOR_RIGHT)
		throw TerminalTooDumbError();
	}

	BindingTerminalCanvas::ColumnToStrategy
	BindingTerminalCanvas::determineColumnToStrategy(TerminalBinding& binding) {
		mapStrategy(OO_COLUMN_TO, CTS_COLUMN_TO)
		mapStrategy(OO_COLUMN_BY, CTS_COLUMN_BY)
		mapStrategy(OO_MOVE_TO, CTS_MOVE_TO)
		mapStrategy(OO_MOVE_BY, CTS_MOVE_BY)
		mapStrategy2(OO_CURSOR_LEFT_BY, OO_CURSOR_RIGHT_BY, CTS_CURSOR_LEFT_BY_RIGHT_BY)
		mapStrategy2(OO_CURSOR_LEFT_BY, OO_CURSOR_RIGHT, CTS_CURSOR_LEFT_BY_RIGHT)
		mapStrategy2(OO_CURSOR_LEFT, OO_CURSOR_RIGHT_BY, CTS_CURSOR_LEFT_RIGHT_BY)
		mapStrategy2(OO_CURSOR_LEFT, OO_CURSOR_RIGHT, CTS_CURSOR_LEFT_RIGHT)
		throw TerminalTooDumbError();
	}

	BindingTerminalCanvas::CursorMotionStrategy
	BindingTerminalCanvas::determineCursorLeftStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CURSOR_LEFT, CMS_CURSOR_DIRECTION)
		mapStrategy(OO_CURSOR_LEFT_BY, CMS_CURSOR_DIRECTION_BY)
		return CMS_DIMENSION_TO;
	}

	BindingTerminalCanvas::CursorMotionStrategy
	BindingTerminalCanvas::determineCursorLeftByStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CURSOR_LEFT_BY, CMS_CURSOR_DIRECTION_BY)
		mapStrategy(OO_CURSOR_LEFT, CMS_CURSOR_DIRECTION)
		return CMS_DIMENSION_TO;
	}

	BindingTerminalCanvas::CursorMotionStrategy
	BindingTerminalCanvas::determineCursorRightStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CURSOR_RIGHT, CMS_CURSOR_DIRECTION)
		mapStrategy(OO_CURSOR_RIGHT_BY, CMS_CURSOR_DIRECTION_BY)
		return CMS_DIMENSION_TO;
	}

	BindingTerminalCanvas::CursorMotionStrategy
	BindingTerminalCanvas::determineCursorRightByStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CURSOR_RIGHT_BY, CMS_CURSOR_DIRECTION_BY)
		mapStrategy(OO_CURSOR_RIGHT, CMS_CURSOR_DIRECTION)
		return CMS_DIMENSION_TO;
	}

	BindingTerminalCanvas::CursorMotionStrategy
	BindingTerminalCanvas::determineCursorUpStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CURSOR_UP, CMS_CURSOR_DIRECTION)
		mapStrategy(OO_CURSOR_UP_BY, CMS_CURSOR_DIRECTION_BY)
		return CMS_DIMENSION_TO;
	}

	BindingTerminalCanvas::CursorMotionStrategy
	BindingTerminalCanvas::determineCursorUpByStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CURSOR_UP_BY, CMS_CURSOR_DIRECTION_BY)
		mapStrategy(OO_CURSOR_UP, CMS_CURSOR_DIRECTION)
		return CMS_DIMENSION_TO;
	}

	BindingTerminalCanvas::CursorMotionStrategy
	BindingTerminalCanvas::determineCursorDownStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CURSOR_DOWN, CMS_CURSOR_DIRECTION)
		mapStrategy(OO_CURSOR_DOWN_BY, CMS_CURSOR_DIRECTION_BY)
		return CMS_DIMENSION_TO;
	}

	BindingTerminalCanvas::CursorMotionStrategy
	BindingTerminalCanvas::determineCursorDownByStrategy(TerminalBinding& binding) {
		mapStrategy(OO_CURSOR_DOWN_BY, CMS_CURSOR_DIRECTION_BY)
		mapStrategy(OO_CURSOR_DOWN, CMS_CURSOR_DIRECTION)
		return CMS_DIMENSION_TO;
	}

#undef mapStrategy
#undef mapStrategy2

	void BindingTerminalCanvas::updateSize() {
		if(binding.hasSizeChanged()) {
			currentSize = binding.getSize();
			if(cursorRow >= currentSize.height)
				cursorRow = currentSize.height - 1u;
			if(cursorColumn >= currentSize.width)
				cursorColumn = currentSize.width - 1u;
			sizeChangePending = true;
		}
	}

	bool BindingTerminalCanvas::hasSizeChanged() {
		if(sizeChangePending)
			return true;
		if(!binding.hasSizeChanged())
			return false;
		currentSize = binding.getSize();
		if(cursorRow >= currentSize.height)
			cursorRow = currentSize.height - 1u;
		if(cursorColumn >= currentSize.width)
			cursorColumn = currentSize.width - 1u;
		return true;
	}

	Size BindingTerminalCanvas::getSize() {
		if(binding.hasSizeChanged()) {
			currentSize = binding.getSize();
			if(cursorRow >= currentSize.height)
				cursorRow = currentSize.height - 1u;
			if(cursorColumn >= currentSize.width)
				cursorColumn = currentSize.width - 1u;
		}
		sizeChangePending = false;
		return currentSize;
	}

	unsigned BindingTerminalCanvas::getCursorRow() {
		updateSize();
		return cursorRow;
	}

	unsigned BindingTerminalCanvas::getCursorColumn() {
		updateSize();
		return cursorColumn;
	}

	void BindingTerminalCanvas::getCursorPosition(unsigned& row, unsigned& column) {
		updateSize();
		row = cursorRow;
		column = cursorColumn;
	}

	unsigned BindingTerminalCanvas::getTabSpacing() {
		return binding.getTabSpacing();
	}

	unsigned BindingTerminalCanvas::getTabOffset() {
		return 0u;
	}

	void BindingTerminalCanvas::carriageReturn() {
		if(!cursorColumn)
			return;
		updateSize();
		switch(carriageReturnStrategy) {
			case CRS_CARRIAGE_RETURN:
				if(binding.carriageReturn())
					break;
			case CRS_COLUMN_TO:
				if(binding.columnTo(0u))
					break;
			case CRS_COLUMN_BY:
				if(binding.columnBy(-static_cast<int>(cursorColumn)))
					break;
			case CRS_MOVE_TO:
				if(binding.moveTo(cursorRow, 0u))
					break;
			case CRS_MOVE_BY:
				if(binding.moveBy(0, -static_cast<int>(cursorColumn)))
					break;
			case CRS_CURSOR_LEFT_BY:
				if(binding.cursorLeftBy(cursorColumn))
					break;
			case CRS_CURSOR_LEFT:
				do {
					if(!binding.cursorLeft())
						throw TerminalTooDumbError();
					--cursorColumn;
				} while(cursorColumn);
				break;
			default:
				throw ProgrammingError("Unrecognized CarriageReturnStrategy in "
						"BindingTerminalCanvas::carriageReturn(): "
						+ StringUtils::toString(static_cast<int>(carriageReturnStrategy)));
		}
		cursorColumn = 0u;
	}

	void BindingTerminalCanvas::newLine() {
		updateSize();
		binding.newLine();
		if(cursorRow + 1u < currentSize.height)
			++cursorRow;
		cursorColumn = 0u;
	}

	void BindingTerminalCanvas::homeRow() {
		if(!cursorRow)
			return;
		switch(homeRowStrategy) {
			case HRS_ROW_TO:
				if(binding.rowTo(0u))
					break;
			case HRS_ROW_BY:
				if(binding.rowBy(-static_cast<int>(cursorRow)))
					break;
			case HRS_MOVE_TO:
				if(binding.moveTo(0u, cursorColumn))
					break;
			case HRS_MOVE_BY:
				if(binding.moveBy(-static_cast<int>(cursorRow), 0))
					break;
			case HRS_CURSOR_UP_BY:
				if(binding.cursorUpBy(cursorRow))
					break;
			case HRS_CURSOR_UP:
				do {
					if(!binding.cursorUp())
						throw TerminalTooDumbError();
					--cursorRow;
				} while(cursorRow);
				break;
			case HRS_IMPOSSIBLE:
				throw TerminalTooDumbError();
			default:
				throw ProgrammingError("Unrecognized HomeRowStrategy in BindingTerminalCanvas::homeRow(): "
						+ StringUtils::toString(static_cast<int>(homeRowStrategy)));
		}
		cursorRow = 0u;
	}

	void BindingTerminalCanvas::clearScreen(bool homeCursor) {
		updateSize();
		const unsigned targetRow = currentSize.height - 1u;
		switch(clearScreenStrategy) {
			case CSS_CLEAR_SCREEN:
				if(binding.clearScreen()) {
					cursorRow = cursorColumn = 0u;
					return;
				}
			case CSS_ROW_TO:
				if(cursorRow < targetRow && binding.rowTo(targetRow))
					break;
			case CSS_ROW_BY:
				if(cursorRow < targetRow && binding.rowBy(static_cast<int>(targetRow - cursorRow)))
					break;
			case CSS_MOVE_TO:
				if(cursorRow < targetRow && binding.moveTo(targetRow, 0u))
					break;
			case CSS_MOVE_BY:
				if(cursorRow < targetRow && binding.moveBy(static_cast<int>(targetRow - cursorRow), 0))
					break;
			case CSS_CURSOR_DOWN_BY:
				if(cursorRow < targetRow && binding.cursorDownBy(targetRow - cursorRow))
					break;
			case CSS_NEW_LINE:
				while(cursorRow < targetRow) {
					binding.newLine();
					++cursorRow;
				}
				break;
			default:
				throw ProgrammingError("Unrecognized ClearScreenStrategy in BindingTerminalCanvas::clearScreen(): "
						+ StringUtils::toString(static_cast<int>(clearScreenStrategy)));
		}
		cursorRow = targetRow;
		cursorColumn = 0u;
		unsigned u;
		for(u = 0u; u < currentSize.height; ++u)
			binding.newLine();
		if(homeCursor)
			homeRow();
	}

	void BindingTerminalCanvas::moveTo(unsigned row, unsigned column) {
		updateSize();
		if(row >= currentSize.height)
			row = currentSize.height - 1u;
		if(column >= currentSize.width)
			column = currentSize.width - 1u;
		if(row == cursorRow && column == cursorColumn)
			return;
		switch(moveToStrategy) {
			case MTS_MOVE_TO:
				if(binding.moveTo(row, column)) {
					cursorRow = row;
					cursorColumn = column;
					break;
				}
			case MTS_MOVE_BY:
				if(binding.moveBy(static_cast<int>(row) - static_cast<int>(cursorRow),
						static_cast<int>(column) - static_cast<int>(cursorColumn))) {
					cursorRow = row;
					cursorColumn = column;
					break;
				}
			case MTS_SPLIT:
				rowTo(row);
				columnTo(column);
				break;
			default:
				throw ProgrammingError("Unrecognized MoveToStrategy in BindingTerminalCanvas::moveTo(): "
						+ StringUtils::toString(static_cast<int>(moveToStrategy)));
		}
	}

	void BindingTerminalCanvas::rowTo(unsigned row) {
		updateSize();
		if(row >= currentSize.height)
			row = currentSize.height - 1u;
		if(row == cursorRow)
			return;
		switch(rowToStrategy) {
			case RTS_ROW_TO:
				if(binding.rowTo(row))
					break;
			case RTS_ROW_BY:
				if(binding.rowBy(static_cast<int>(row) - static_cast<int>(cursorRow)))
					break;
			case RTS_MOVE_TO:
				if(binding.moveTo(row, cursorColumn))
					break;
			case RTS_MOVE_BY:
				if(binding.moveBy(static_cast<int>(row) - static_cast<int>(cursorRow), 0))
					break;
			case RTS_CURSOR_UP_BY_DOWN_BY:
				if(row < cursorRow) {
					if(binding.cursorUpBy(cursorRow - row))
						break;
				}
				else {
					if(binding.cursorDownBy(row - cursorRow))
						break;
				}
			case RTS_CURSOR_UP_BY_DOWN:
				if(row < cursorRow) {
					if(binding.cursorUpBy(cursorRow - row))
						break;
				}
				else {
					do {
						if(!binding.cursorDown())
							break;
						++cursorRow;
					} while(cursorRow < row);
					if(cursorRow == row)
						break;
				}
			case RTS_CURSOR_UP_DOWN_BY:
				if(row < cursorRow) {
					do {
						if(!binding.cursorUp())
							break;
						--cursorRow;
					} while(cursorRow > row);
					if(cursorRow == row)
						break;
				}
				else {
					if(binding.cursorDownBy(row - cursorRow))
						break;
				}
			case RTS_CURSOR_UP_BY_NEW_LINE_COLUMN_TO:
			case RTS_CURSOR_UP_BY_NEW_LINE_COLUMN_BY:
			case RTS_CURSOR_UP_BY_NEW_LINE_CURSOR_RIGHT_BY:
			case RTS_CURSOR_UP_BY_NEW_LINE_CURSOR_RIGHT:
				if(row < cursorRow) {
					if(binding.cursorUpBy(cursorRow - row))
						break;
				}
				else {
					unsigned targetColumn = cursorColumn;
					do {
						binding.newLine();
						++cursorRow;
					} while(cursorRow < row);
					cursorColumn = 0u;
					columnTo(targetColumn);
					break;
				}
			case RTS_CURSOR_UP_DOWN:
				if(row < cursorRow) {
					do {
						if(!binding.cursorUp())
							break;
						--cursorRow;
					} while(cursorRow > row);
					if(cursorRow == row)
						break;
				}
				else {
					do {
						if(!binding.cursorDown())
							break;
						++cursorRow;
					} while(cursorRow < row);
					if(cursorRow == row)
						break;
				}
			case RTS_CURSOR_UP_NEW_LINE_COLUMN_TO:
			case RTS_CURSOR_UP_NEW_LINE_COLUMN_BY:
			case RTS_CURSOR_UP_NEW_LINE_CURSOR_RIGHT_BY:
			case RTS_CURSOR_UP_NEW_LINE_CURSOR_RIGHT:
				if(row < cursorRow) {
					do {
						if(!binding.cursorUp())
							break;
						--cursorRow;
					} while(cursorRow > row);
					if(cursorRow == row)
						break;
				}
				else {
					unsigned targetColumn = cursorColumn;
					do {
						binding.newLine();
						++cursorRow;
					} while(cursorRow < row);
					cursorColumn = 0u;
					columnTo(targetColumn);
					break;
				}
				throw TerminalTooDumbError();
			default:
				throw ProgrammingError("Unrecognized RowToStrategy in BindingTerminalCanvas::rowTo(): "
						+ StringUtils::toString(static_cast<int>(rowToStrategy)));
		}
		cursorRow = row;
	}

	void BindingTerminalCanvas::columnTo(unsigned column) {
		updateSize();
		if(column >= currentSize.width)
			column = currentSize.width - 1u;
		if(column == cursorColumn)
			return;
		switch(columnToStrategy) {
			case CTS_COLUMN_TO:
				if(binding.columnTo(column))
					break;
			case CTS_COLUMN_BY:
				if(binding.columnBy(static_cast<int>(column) - static_cast<int>(cursorColumn)))
					break;
			case CTS_MOVE_TO:
				if(binding.moveTo(cursorRow, column))
					break;
			case CTS_MOVE_BY:
				if(binding.moveBy(0, static_cast<int>(column) - static_cast<int>(cursorColumn)))
					break;
			case CTS_CURSOR_LEFT_BY_RIGHT_BY:
				if(column < cursorColumn) {
					if(binding.cursorLeftBy(cursorColumn - column))
						break;
				}
				else {
					if(binding.cursorRightBy(column - cursorColumn))
						break;
				}
			case CTS_CURSOR_LEFT_BY_RIGHT:
				if(column < cursorColumn) {
					if(binding.cursorLeftBy(cursorColumn - column))
						break;
				}
				else {
					do {
						if(!binding.cursorRight())
							break;
						++cursorColumn;
					} while(cursorColumn < column);
					if(cursorColumn == column)
						break;
				}
			case CTS_CURSOR_LEFT_RIGHT_BY:
				if(column < cursorColumn) {
					do {
						if(binding.cursorLeft())
							break;
						--cursorColumn;
					} while(cursorColumn > column);
					if(cursorColumn == column)
						break;
				}
				else {
					if(binding.cursorRightBy(column - cursorColumn))
						break;
				}
			case CTS_CURSOR_LEFT_RIGHT:
				if(column < cursorColumn) {
					do {
						if(binding.cursorLeft())
							break;
						--cursorColumn;
					} while(cursorColumn > column);
					if(cursorColumn == column)
						break;
				}
				else {
					do {
						if(!binding.cursorRight())
							break;
						++cursorColumn;
					} while(cursorColumn < column);
					if(cursorColumn == column)
						break;
				}
				throw TerminalTooDumbError();
			default:
				throw ProgrammingError("Unrecognized ColumnToStrategy in BindingTerminalCanvas::columnTo(): "
						+ StringUtils::toString(static_cast<int>(columnToStrategy)));
		}
		cursorColumn = column;
	}

	void BindingTerminalCanvas::moveBy(int rows, int columns) {
		updateSize();
		int targetRow = static_cast<int>(cursorRow) + rows;
		int targetColumn = static_cast<int>(cursorColumn) + columns;
		moveTo(targetRow < 0 ? 0u : static_cast<unsigned>(targetRow),
				targetColumn < 0 ? 0u : static_cast<unsigned>(targetColumn));
	}

	void BindingTerminalCanvas::rowBy(int rows) {
		updateSize();
		int targetRow = static_cast<int>(cursorRow) + rows;
		rowTo(targetRow < 0 ? 0u : static_cast<unsigned>(targetRow));
	}

	void BindingTerminalCanvas::columnBy(int columns) {
		updateSize();
		int targetColumn = static_cast<int>(cursorColumn) + columns;
		columnTo(targetColumn < 0 ? 0u : static_cast<unsigned>(targetColumn));
	}

	void BindingTerminalCanvas::cursorDown() {
		updateSize();
		if(cursorRow + 1u >= currentSize.height)
			return;
		switch(cursorDownStrategy) {
			case CMS_CURSOR_DIRECTION:
				if(binding.cursorDown()) {
					++cursorRow;
					break;
				}
			case CMS_CURSOR_DIRECTION_BY:
				if(binding.cursorDownBy(1u)) {
					++cursorRow;
					break;
				}
			case CMS_DIMENSION_TO:
				rowTo(cursorRow + 1u);
				break;
			default:
				throw ProgrammingError("Unrecognized CursorMotionStrategy in BindingTerminalCanvas::cursorDown(): "
						+ StringUtils::toString(static_cast<int>(cursorDownStrategy)));
		}
	}

	void BindingTerminalCanvas::cursorDownBy(unsigned rows) {
		updateSize();
		if(cursorRow + rows >= currentSize.height)
			rows = currentSize.height - cursorRow - 1u;
		if(!rows)
			return;
		switch(cursorDownByStrategy) {
			case CMS_CURSOR_DIRECTION_BY:
				if(binding.cursorDownBy(rows)) {
					cursorRow += rows;
					break;
				}
			case CMS_CURSOR_DIRECTION:
				do {
					if(!binding.cursorDown())
						break;
					++cursorRow;
					--rows;
				} while(rows);
				if(!rows)
					break;
			case CMS_DIMENSION_TO:
				rowTo(cursorRow + rows);
				break;
			default:
				throw ProgrammingError("Unrecognized CursorMotionStrategy in BindingTerminalCanvas::cursorDownBy(): "
						+ StringUtils::toString(static_cast<int>(cursorDownByStrategy)));
		}
	}

	void BindingTerminalCanvas::cursorUp() {
		updateSize();
		if(!cursorRow)
			return;
		switch(cursorUpStrategy) {
			case CMS_CURSOR_DIRECTION:
				if(binding.cursorUp()) {
					--cursorRow;
					break;
				}
			case CMS_CURSOR_DIRECTION_BY:
				if(binding.cursorUpBy(1u)) {
					--cursorRow;
					break;
				}
			case CMS_DIMENSION_TO:
				rowTo(cursorRow - 1u);
				break;
			default:
				throw ProgrammingError("Unrecognized CursorMotionStrategy in BindingTerminalCanvas::cursorUp(): "
						+ StringUtils::toString(static_cast<int>(cursorUpStrategy)));
		}
	}

	void BindingTerminalCanvas::cursorUpBy(unsigned rows) {
		updateSize();
		if(rows > cursorRow)
			rows = cursorRow;
		if(!rows)
			return;
		switch(cursorUpByStrategy) {
			case CMS_CURSOR_DIRECTION_BY:
				if(binding.cursorUpBy(rows)) {
					cursorRow -= rows;
					break;
				}
			case CMS_CURSOR_DIRECTION:
				do {
					if(!binding.cursorUp())
						break;
					--cursorRow;
					--rows;
				} while(rows);
				if(!rows)
					break;
			case CMS_DIMENSION_TO:
				rowTo(cursorRow - rows);
				break;
			default:
				throw ProgrammingError("Unrecognized CursorMotionStrategy in BindingTerminalCanvas::cursorUpBy(): "
						+ StringUtils::toString(static_cast<int>(cursorUpByStrategy)));
		}
	}

	void BindingTerminalCanvas::cursorLeft() {
		updateSize();
		if(!cursorColumn)
			return;
		switch(cursorLeftStrategy) {
			case CMS_CURSOR_DIRECTION:
				if(binding.cursorLeft()) {
					--cursorColumn;
					break;
				}
			case CMS_CURSOR_DIRECTION_BY:
				if(binding.cursorLeftBy(1u)) {
					--cursorColumn;
					break;
				}
			case CMS_DIMENSION_TO:
				columnTo(cursorColumn - 1u);
				break;
			default:
				throw ProgrammingError("Unrecognized CursorMotionStrategy in BindingTerminalCanvas::cursorLeft(): "
						+ StringUtils::toString(static_cast<int>(cursorLeftStrategy)));
		}
	}

	void BindingTerminalCanvas::cursorLeftBy(unsigned columns) {
		updateSize();
		if(columns > cursorColumn)
			columns = cursorColumn;
		if(!columns)
			return;
		switch(cursorLeftByStrategy) {
			case CMS_CURSOR_DIRECTION_BY:
				if(binding.cursorLeftBy(columns)) {
					cursorColumn -= columns;
					break;
				}
			case CMS_CURSOR_DIRECTION:
				do {
					if(!binding.cursorLeft())
						break;
					--cursorColumn;
					--columns;
				} while(columns);
				if(!columns)
					break;
			case CMS_DIMENSION_TO:
				columnTo(cursorColumn - columns);
				break;
			default:
				throw ProgrammingError("Unrecognized CursorMotionStrategy in BindingTerminalCanvas::cursorLeftBy(): "
						+ StringUtils::toString(static_cast<int>(cursorLeftByStrategy)));
		}
	}

	void BindingTerminalCanvas::cursorRight() {
		updateSize();
		if(cursorColumn + 1u >= currentSize.width)
			return;
		switch(cursorRightStrategy) {
			case CMS_CURSOR_DIRECTION:
				if(binding.cursorRight()) {
					++cursorColumn;
					break;
				}
			case CMS_CURSOR_DIRECTION_BY:
				if(binding.cursorRightBy(1u)) {
					++cursorColumn;
					break;
				}
			case CMS_DIMENSION_TO:
				columnTo(cursorColumn + 1u);
				break;
			default:
				throw ProgrammingError("Unrecognized CursorMotionStrategy in BindingTerminalCanvas::cursorRight(): "
						+ StringUtils::toString(static_cast<int>(cursorRightStrategy)));
		}
	}

	void BindingTerminalCanvas::cursorRightBy(unsigned columns) {
		updateSize();
		if(cursorColumn + columns >= currentSize.width)
			columns = currentSize.width - cursorColumn - 1u;
		if(!columns)
			return;
		switch(cursorRightByStrategy) {
			case CMS_CURSOR_DIRECTION_BY:
				if(binding.cursorRightBy(columns)) {
					cursorColumn += columns;
					break;
				}
			case CMS_CURSOR_DIRECTION:
				do {
					if(!binding.cursorRight())
						break;
					++cursorColumn;
					--columns;
				} while(columns);
				if(!columns)
					break;
			case CMS_DIMENSION_TO:
				columnTo(cursorColumn + columns);
				break;
			default:
				throw ProgrammingError("Unrecognized CursorMotionStrategy in BindingTerminalCanvas::cursorRightBy(): "
						+ StringUtils::toString(static_cast<int>(cursorRightByStrategy)));
		}
	}

	void BindingTerminalCanvas::setCursorVisible(bool visible) {
		binding.setCursorVisible(visible);
	}

	unsigned BindingTerminalCanvas::setForegroundColor(unsigned color) {
		return binding.setForegroundColor(color);
	}

	unsigned BindingTerminalCanvas::setBackgroundColor(unsigned color) {
		return binding.setBackgroundColor(color);
	}

	int BindingTerminalCanvas::setTextAttributes(int attributes) {
		return binding.setTextAttributes(attributes);
	}

	int BindingTerminalCanvas::addTextAttributes(int attributes) {
		return binding.addTextAttributes(attributes);
	}

	int BindingTerminalCanvas::removeTextAttributes(int attributes) {
		return binding.removeTextAttributes(attributes);
	}

	void BindingTerminalCanvas::resetTextAttributes() {
		binding.resetTextAttributes();
	}

	KeySym BindingTerminalCanvas::read(bool blocking) {
		KeySym key(binding.read(blocking));
		if(key.getType() == KeySym::T_WINCH) {
			currentSize = binding.getSize();
			if(cursorRow >= currentSize.height)
				cursorRow = currentSize.height - 1u;
			if(cursorColumn >= currentSize.width)
				cursorColumn = currentSize.width - 1u;
		}
		return key;
	}

	void BindingTerminalCanvas::prepareForWrite() {
		updateSize();
	}

	void BindingTerminalCanvas::writeTab() {
		binding.write('\t');
		if(cursorColumn == currentSize.width) {
			if(cursorRow + 1u < currentSize.height)
				++cursorRow;
			cursorColumn = 0u;
		}
		else {
			unsigned tabs = binding.getTabSpacing();
			if(!tabs)
				tabs = 1u;
			cursorColumn = (cursorColumn / tabs + 1u) * tabs;
			if(cursorColumn > currentSize.width)
				cursorColumn = currentSize.width;
		}
	}

	void BindingTerminalCanvas::writeNonControl(char c) {
		if(cursorColumn >= currentSize.width && !binding.hasAutoRightMargin())
			newLine();
		binding.write(c);
		if(cursorColumn >= currentSize.width) {
			if(cursorRow + 1u < currentSize.height)
				++cursorRow;
			cursorColumn = 1u;
		}
		else
			++cursorColumn;
	}

	void BindingTerminalCanvas::writeNonControl(const char* chars, MemorySize count) {
		MemorySize start = static_cast<MemorySize>(0u);
		while(start < count) {
			if(cursorColumn >= currentSize.width)
				newLine();
			MemorySize rest = static_cast<MemorySize>(currentSize.width - cursorColumn);
			if(rest > count - start)
				rest = count - start;
			binding.write(string(chars + start, static_cast<string::size_type>(rest)));
			cursorColumn += static_cast<unsigned>(rest);
			start += rest;
		}
	}

	void BindingTerminalCanvas::writeNonControl(Char16 c) {
		if(cursorColumn >= currentSize.width && !binding.hasAutoRightMargin())
			newLine();
		binding.write(c);
		if(cursorColumn >= currentSize.width) {
			if(cursorRow + 1u < currentSize.height)
				++cursorRow;
			cursorColumn = 1u;
		}
		else
			++cursorColumn;
	}

	void BindingTerminalCanvas::writeNonControl(const Char16* chars, MemorySize count) {
		MemorySize start = static_cast<MemorySize>(0u);
		while(start < count) {
			if(cursorColumn >= currentSize.width)
				newLine();
			MemorySize rest = static_cast<MemorySize>(currentSize.width - cursorColumn);
			if(rest > count - start)
				rest = count - start;
			binding.write(String16(chars + start, static_cast<String16::size_type>(rest)));
			cursorColumn += static_cast<unsigned>(rest);
			start += rest;
		}
	}

	void BindingTerminalCanvas::writeSymbol(BoxSymbol symbol) {
		updateSize();
		if(cursorColumn >= currentSize.width && !binding.hasAutoRightMargin())
			newLine();
		binding.write(symbol);
		if(cursorColumn >= currentSize.width) {
			if(cursorRow + 1u < currentSize.height)
				++cursorRow;
			cursorColumn = 1u;
		}
		else
			++cursorColumn;
	}

	void BindingTerminalCanvas::flush() {
		binding.flush();
	}

	void BindingTerminalCanvas::closeCanvas() {
		binding.setInputMode(TerminalBinding::IM_COOKED);
		binding.stopTerminalUse();
	}

}}

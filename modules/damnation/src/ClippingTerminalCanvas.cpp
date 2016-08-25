#include "ClippingTerminalCanvas.hpp"
#include "ClipStackUnderflowError.hpp"
#include "OriginStackUnderflowError.hpp"

using std::string;
using redengine::text::Char16;
using redengine::util::MemorySize;

namespace redengine {
namespace damnation {

	ClippingTerminalCanvas::ClippingTerminalCanvas(TerminalCanvas& terminal)
			: terminal(terminal), currentSize(terminal.getSize()), sizeChangePending(false) {
		unsigned row, column;
		terminal.getCursorPosition(row, column);
		cursorRow = static_cast<int>(row);
		cursorColumn = static_cast<int>(column);
	}

	ClippingTerminalCanvas::ClippingTerminalCanvas(const ClippingTerminalCanvas& canvas)
			: TerminalCanvas(canvas), terminal(canvas.terminal), currentSize(canvas.currentSize),
			sizeChangePending(canvas.sizeChangePending), clipStack(canvas.clipStack),
			originStack(canvas.originStack), cursorRow(canvas.cursorRow), cursorColumn(canvas.cursorColumn) {}

	ClippingTerminalCanvas::~ClippingTerminalCanvas() {}

#define CURRENT_ORIGIN (originStack.empty() ? Point::ORIGIN : originStack.back())

	void ClippingTerminalCanvas::pushClip(const Rectangle& mask) {
		if(clipStack.empty())
			clipStack.push_back(mask + CURRENT_ORIGIN);
		else
			clipStack.push_back((mask + CURRENT_ORIGIN).intersect(clipStack.back()));
	}

	void ClippingTerminalCanvas::popClip() {
		if(clipStack.empty())
			throw ClipStackUnderflowError();
		clipStack.pop_back();
	}

	void ClippingTerminalCanvas::clearClip() {
		clipStack.clear();
	}

	Rectangle ClippingTerminalCanvas::getCurrentClipMask() {
		updateSize();
		Rectangle trect(0, 0, currentSize.width, currentSize.height);
		return clipStack.empty() ? trect : trect.intersect(clipStack.back());
	}

	void ClippingTerminalCanvas::pushOrigin(const Point& origin) {
		if(originStack.empty())
			originStack.push_back(origin);
		else
			originStack.push_back(origin + originStack.back());
		if(origin != Point::ORIGIN)
			sizeChangePending = true;
	}

	void ClippingTerminalCanvas::popOrigin() {
		if(originStack.empty())
			throw OriginStackUnderflowError();
		const Point popped(originStack.back());
		originStack.pop_back();
		if(popped != CURRENT_ORIGIN)
			sizeChangePending = true;
	}

	void ClippingTerminalCanvas::clearOrigin() {
		if(!originStack.empty() && originStack.back() != Point::ORIGIN)
			sizeChangePending = true;
		originStack.clear();
	}

	const Point& ClippingTerminalCanvas::getCurrentOrigin() const {
		return CURRENT_ORIGIN;
	}

	void ClippingTerminalCanvas::updateSize() {
		if(terminal.hasSizeChanged()) {
			currentSize = terminal.getSize();
			sizeChangePending = true;
		}
	}

	bool ClippingTerminalCanvas::hasSizeChanged() {
		if(sizeChangePending)
			return true;
		if(!terminal.hasSizeChanged())
			return false;
		currentSize = terminal.getSize();
		return true;
	}

	Size ClippingTerminalCanvas::getSize() {
		if(terminal.hasSizeChanged())
			currentSize = terminal.getSize();
		sizeChangePending = false;
		const Point origin(CURRENT_ORIGIN);
		return Size(
			origin.column >= static_cast<int>(currentSize.width) ? 1u
					: static_cast<unsigned>(static_cast<int>(currentSize.width) - origin.column),
			origin.row >= static_cast<int>(currentSize.height) ? 1u
					: static_cast<unsigned>(static_cast<int>(currentSize.height) - origin.row)
		);
	}

	unsigned ClippingTerminalCanvas::getCursorRow() {
		int orow = CURRENT_ORIGIN.row;
		return orow > cursorRow ? 0u : static_cast<unsigned>(cursorRow - orow);
	}

	unsigned ClippingTerminalCanvas::getCursorColumn() {
		int ocolumn = CURRENT_ORIGIN.column;
		return ocolumn > cursorColumn ? 0u : static_cast<unsigned>(cursorColumn - ocolumn);
	}

	void ClippingTerminalCanvas::getCursorPosition(unsigned& row, unsigned& column) {
		const Point origin(CURRENT_ORIGIN);
		row = origin.row > cursorRow ? 0u : static_cast<unsigned>(cursorRow - origin.row);
		column = origin.column > cursorColumn ? 0u : static_cast<unsigned>(cursorColumn - origin.column);
	}

	unsigned ClippingTerminalCanvas::getTabSpacing() {
		return terminal.getTabSpacing();
	}

	unsigned ClippingTerminalCanvas::getTabOffset() {
		const int toffset = static_cast<int>(terminal.getTabOffset());
		const unsigned tspacing = terminal.getTabSpacing();
		const int ocolumn = CURRENT_ORIGIN.column;
		// realTab(n) = tspacing * n + toffset
		// virtualTab(n) = tspacing * n + toffset - ocolumn
		// Thus, we need to return toffset - ocolumn (mod tspacing)
		if(toffset >= ocolumn)
			return static_cast<unsigned>(toffset - ocolumn) % tspacing;
		else
			return (tspacing - static_cast<unsigned>(ocolumn - toffset) % tspacing) % tspacing;
	}

	void ClippingTerminalCanvas::carriageReturn() {
		int targetColumn = CURRENT_ORIGIN.column;
		if(!targetColumn)
			terminal.carriageReturn();
		else
			terminal.columnTo(targetColumn < 0 ? 0u : static_cast<unsigned>(targetColumn));
		cursorColumn = targetColumn;
	}

	void ClippingTerminalCanvas::newLine() {
		int targetRow = cursorRow + 1;
		int targetColumn = CURRENT_ORIGIN.column;
		terminal.moveTo(
			targetRow < 0 ? 0u : static_cast<unsigned>(targetRow),
			targetColumn < 0 ? 0u : static_cast<unsigned>(targetColumn)
		);
		cursorRow = targetRow;
		cursorColumn = targetColumn;
	}

	void ClippingTerminalCanvas::clearScreen(bool) {
		Rectangle mask(getCurrentClipMask());
		if(mask == currentSize) {
			terminal.clearScreen(true);
			cursorRow = cursorColumn = 0;
			return;
		}
		terminal.fill(mask, ' ');
		terminal.moveTo(
			mask.row < 0 ? 0u : static_cast<unsigned>(mask.row),
			mask.column < 0 ? 0u : static_cast<unsigned>(mask.column)
		);
		cursorRow = mask.row;
		cursorColumn = mask.column;
	}

	void ClippingTerminalCanvas::moveTo(unsigned row, unsigned column) {
		Point origin(CURRENT_ORIGIN);
		origin.row += static_cast<int>(row);
		origin.column += static_cast<int>(column);
		terminal.moveTo(
			origin.row < 0 ? 0u : static_cast<unsigned>(origin.row),
			origin.column < 0 ? 0u : static_cast<unsigned>(origin.column)
		);
		cursorRow = origin.row;
		cursorColumn = origin.column;
	}

	void ClippingTerminalCanvas::rowTo(unsigned row) {
		int targetRow = CURRENT_ORIGIN.row + static_cast<int>(row);
		terminal.rowTo(targetRow < 0 ? 0u : static_cast<unsigned>(targetRow));
		cursorRow = targetRow;
	}

	void ClippingTerminalCanvas::columnTo(unsigned column) {
		int targetColumn = CURRENT_ORIGIN.column + static_cast<int>(column);
		terminal.columnTo(targetColumn < 0 ? 0u : static_cast<unsigned>(targetColumn));
		cursorColumn = targetColumn;
	}

	void ClippingTerminalCanvas::moveBy(int rows, int columns) {
		terminal.moveBy(rows, columns);
		cursorRow += rows;
		cursorColumn += columns;
	}

	void ClippingTerminalCanvas::rowBy(int rows) {
		terminal.rowBy(rows);
		cursorRow += rows;
	}

	void ClippingTerminalCanvas::columnBy(int columns) {
		terminal.columnBy(columns);
		cursorColumn += columns;
	}

	void ClippingTerminalCanvas::cursorDown() {
		terminal.cursorDown();
		++cursorRow;
	}

	void ClippingTerminalCanvas::cursorDownBy(unsigned rows) {
		terminal.cursorDownBy(rows);
		cursorRow += static_cast<int>(rows);
	}

	void ClippingTerminalCanvas::cursorUp() {
		terminal.cursorUp();
		--cursorRow;
	}

	void ClippingTerminalCanvas::cursorUpBy(unsigned rows) {
		terminal.cursorUpBy(rows);
		cursorRow -= static_cast<int>(rows);
	}

	void ClippingTerminalCanvas::cursorLeft() {
		terminal.cursorLeft();
		--cursorColumn;
	}

	void ClippingTerminalCanvas::cursorLeftBy(unsigned columns) {
		terminal.cursorLeftBy(columns);
		cursorColumn -= static_cast<int>(columns);
	}

	void ClippingTerminalCanvas::cursorRight() {
		terminal.cursorRight();
		++cursorColumn;
	}

	void ClippingTerminalCanvas::cursorRightBy(unsigned columns) {
		terminal.cursorRightBy(columns);
		cursorColumn += static_cast<int>(columns);
	}

	void ClippingTerminalCanvas::setCursorVisible(bool visible) {
		terminal.setCursorVisible(visible);
	}

	unsigned ClippingTerminalCanvas::setForegroundColor(unsigned color) {
		return terminal.setForegroundColor(color);
	}

	unsigned ClippingTerminalCanvas::setBackgroundColor(unsigned color) {
		return terminal.setBackgroundColor(color);
	}

	int ClippingTerminalCanvas::setTextAttributes(int attributes) {
		return terminal.setTextAttributes(attributes);
	}

	int ClippingTerminalCanvas::addTextAttributes(int attributes) {
		return terminal.addTextAttributes(attributes);
	}

	int ClippingTerminalCanvas::removeTextAttributes(int attributes) {
		return terminal.removeTextAttributes(attributes);
	}

	void ClippingTerminalCanvas::resetTextAttributes() {
		terminal.resetTextAttributes();
	}

	KeySym ClippingTerminalCanvas::read(bool blocking) {
		KeySym key(terminal.read(blocking));
		if(key.getType() == KeySym::T_WINCH) {
			currentSize = terminal.getSize();
			sizeChangePending = true;
		}
		return key;
	}

	void ClippingTerminalCanvas::prepareForWrite() {
		updateSize();
		terminal.prepareForWrite();
	}

	void ClippingTerminalCanvas::writeTab() {
		const int tabs = static_cast<int>(terminal.getTabSpacing());
		const int offset = static_cast<int>(terminal.getTabOffset());
		const int column = cursorColumn - offset;
		int targetColumn;
		if(column < 0)
			targetColumn = (-((-column + tabs - 1) / tabs) + 1) * tabs + offset;
		else
			targetColumn = (column / tabs + 1) * tabs + offset;
		terminal.columnTo(targetColumn < 0 ? 0u : static_cast<unsigned>(targetColumn));
		cursorColumn = targetColumn;
	}

	void ClippingTerminalCanvas::writeNonControl(char c) {
		if(getCurrentClipMask().contains(Point(cursorRow, cursorColumn)))
			terminal.writeNonControl(c);
		else
			terminal.cursorRight();
		++cursorColumn;
	}

	void ClippingTerminalCanvas::writeNonControl(const char* chars, const MemorySize count) {
		if(!count)
			return;
		const Rectangle mask(getCurrentClipMask());
		if(cursorRow >= mask.row && cursorRow < mask.row + static_cast<int>(mask.height)) {
			const MemorySize cutLeft = static_cast<MemorySize>(cursorColumn < mask.column
					? mask.column - cursorColumn : 0);
			if(cutLeft < count) {
				const int lastColumn = cursorColumn + static_cast<int>(count);
				const int rightEdge = mask.column + static_cast<int>(mask.width);
				const MemorySize cutRight = static_cast<MemorySize>(lastColumn > rightEdge
						? lastColumn - rightEdge : 0);
				const MemorySize restCount = count - cutLeft;
				if(cutRight < restCount) {
					if(cutLeft)
						terminal.cursorRightBy(static_cast<unsigned>(cutLeft));
					terminal.writeNonControl(chars + cutLeft, count - cutLeft - cutRight);
				}
			}
		}
		cursorColumn += static_cast<int>(count);
		terminal.columnTo(cursorColumn < 0 ? 0u : static_cast<unsigned>(cursorColumn));
	}

	void ClippingTerminalCanvas::writeNonControl(Char16 c) {
		if(getCurrentClipMask().contains(Point(cursorRow, cursorColumn)))
			terminal.writeNonControl(c);
		else
			terminal.cursorRight();
		++cursorColumn;
	}

	void ClippingTerminalCanvas::writeNonControl(const Char16* chars, MemorySize count) {
		if(!count)
			return;
		const Rectangle mask(getCurrentClipMask());
		if(cursorRow >= mask.row && cursorRow < mask.row + static_cast<int>(mask.height)) {
			const MemorySize cutLeft = static_cast<MemorySize>(cursorColumn < mask.column
					? mask.column - cursorColumn : 0);
			if(cutLeft < count) {
				const int lastColumn = cursorColumn + static_cast<int>(count);
				const int rightEdge = mask.column + static_cast<int>(mask.width);
				const MemorySize cutRight = static_cast<MemorySize>(lastColumn > rightEdge
						? lastColumn - rightEdge : 0);
				const MemorySize restCount = count - cutLeft;
				if(cutRight < restCount) {
					if(cutLeft)
						terminal.cursorRightBy(static_cast<unsigned>(cutLeft));
					terminal.writeNonControl(chars + cutLeft, count - cutLeft - cutRight);
				}
			}
		}
		cursorColumn += static_cast<int>(count);
		terminal.columnTo(cursorColumn < 0 ? 0u : static_cast<unsigned>(cursorColumn));
	}

	void ClippingTerminalCanvas::writeSymbol(BoxSymbol symbol) {
		updateSize();
		if(getCurrentClipMask().contains(Point(cursorRow, cursorColumn)))
			terminal.write(symbol);
		else
			terminal.cursorRight();
		++cursorColumn;
	}

	void ClippingTerminalCanvas::flush() {
		terminal.flush();
	}

	void ClippingTerminalCanvas::closeCanvas() {
		terminal.closeCanvas();
	}

}}

#include "Rectangle.hpp"
#include "TerminalCanvas.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::util::MemorySize;

namespace redengine {
namespace damnation {

	TerminalCanvas::TerminalCanvas() {}

	TerminalCanvas::TerminalCanvas(const TerminalCanvas&) {}

	TerminalCanvas::~TerminalCanvas() {}

	void TerminalCanvas::write(char c) {
		switch(c) {
			case '\t':
				prepareForWrite();
				writeTab();
				break;
			case '\r':
				carriageReturn();
				break;
			case '\n':
				newLine();
				break;
			default:
				if(c >= ' ') {
					prepareForWrite();
					writeNonControl(c);
				}
				break;
		}
	}

	void TerminalCanvas::write(const string& data) {
		if(data.empty())
			return;
		prepareForWrite();
		string::size_type pos, length = data.length(), old = static_cast<string::size_type>(0u);
		const char* chars = data.data();
		for(;;) {
			for(pos = old; pos < length; ++pos) {
				if(chars[pos] < ' ')
					break;
			}
			if(pos == length) {
				if(old < length)
					writeNonControl(chars + old, static_cast<MemorySize>(length - old));
				break;
			}
			if(pos > old)
				writeNonControl(chars + old, static_cast<MemorySize>(pos - old));
			switch(chars[pos]) {
				case '\t':
					writeTab();
					break;
				case '\r':
					carriageReturn();
					break;
				case '\n':
					newLine();
					break;
			}
			old = pos + static_cast<string::size_type>(1u);
		}
	}

	void TerminalCanvas::write(Char16 c) {
		switch(c) {
			case static_cast<Char16>('\t'):
				prepareForWrite();
				writeTab();
				break;
			case static_cast<Char16>('\r'):
				carriageReturn();
				break;
			case static_cast<Char16>('\n'):
				newLine();
				break;
			default:
				if(c >= static_cast<Char16>(' ')) {
					prepareForWrite();
					writeNonControl(c);
				}
				break;
		}
	}

	void TerminalCanvas::write(const String16& data) {
		if(data.empty())
			return;
		prepareForWrite();
		String16::size_type pos, length = data.length(), old = static_cast<String16::size_type>(0u);
		const Char16* chars = data.data();
		for(;;) {
			for(pos = old; pos < length; ++pos) {
				if(chars[pos] < static_cast<Char16>(' '))
					break;
			}
			if(pos == length) {
				if(old < length)
					writeNonControl(chars + old, static_cast<MemorySize>(length - old));
				break;
			}
			if(pos > old)
				writeNonControl(chars + old, static_cast<MemorySize>(pos - old));
			switch(chars[pos]) {
				case static_cast<Char16>('\t'):
					writeTab();
					break;
				case static_cast<Char16>('\r'):
					carriageReturn();
					break;
				case static_cast<Char16>('\n'):
					newLine();
					break;
			}
			old = pos + static_cast<String16::size_type>(1u);
		}
	}

	static void _makeFillRect(const Rectangle& area, const Size& size,
			unsigned& row, unsigned& column, unsigned& width, unsigned& height) {
		row = area.row < 0 ? 0u : static_cast<unsigned>(area.row);
		if(row >= size.height)
			row = size.height - 1u;
		int sbottom = area.row + static_cast<int>(area.height);
		unsigned bottom = sbottom < 0 ? 0u : static_cast<unsigned>(sbottom);
		if(bottom > size.height)
			bottom = size.height;
		column = area.column < 0 ? 0u : static_cast<unsigned>(area.column);
		if(column >= size.width)
			column = size.width - 1u;
		int sright = area.column + static_cast<int>(area.width);
		unsigned right = sright < 0 ? 0u : static_cast<unsigned>(sright);
		if(right > size.width)
			right = size.width;
		width = right - column;
		height = bottom - row;
	}

	void TerminalCanvas::makeFillRect(const Rectangle& area,
			unsigned& row, unsigned& column, unsigned& width, unsigned& height) {
		const Size size(getSize());
		_makeFillRect(area, size, row, column, width, height);
	}

	void TerminalCanvas::fill(const Rectangle& area, char c) {
		if(!area.width || !area.height)
			return;
		unsigned row, column, width, height;
		makeFillRect(area, row, column, width, height);
		if(!height)
			return;
		unsigned u;
		switch(width) {
			case 0u:
				break;
			case 1u:
				for(u = 0u; u < height; ++u) {
					moveTo(row + u, column);
					write(c);
				}
				break;
			default:
				{
					string line(static_cast<string::size_type>(width), c);
					for(u = 0u; u < height; ++u) {
						moveTo(row + u, column);
						write(line);
					}
				}
				break;
		}
	}

	void TerminalCanvas::fill(const Rectangle& area, Char16 c) {
		if(!area.width || !area.height)
			return;
		unsigned row, column, width, height;
		makeFillRect(area, row, column, width, height);
		if(!height)
			return;
		unsigned u;
		switch(width) {
			case 0u:
				break;
			case 1u:
				for(u = 0u; u < height; ++u) {
					moveTo(row + u, column);
					write(c);
				}
				break;
			default:
				{
					String16 line(static_cast<String16::size_type>(width), c);
					for(u = 0u; u < height; ++u) {
						moveTo(row + u, column);
						write(line);
					}
				}
				break;
		}
	}

	template<typename CharT>
	void _termcan_hline(TerminalCanvas& canvas, unsigned row, unsigned column, unsigned length,
			CharT line, CharT leftEnd, CharT rightEnd) {
		if(!length)
			return;
		canvas.moveTo(row, column);
		if(length == 1u) {
			canvas.write(line);
			return;
		}
		canvas.write(leftEnd);
		for(length -= 2u; length; --length)
			canvas.write(line);
		canvas.write(rightEnd);
	}

	template<typename CharT>
	void _termcan_vline(TerminalCanvas& canvas, unsigned row, unsigned column, unsigned length,
			CharT line, CharT topEnd, CharT bottomEnd) {
		if(!length)
			return;
		unsigned bottom = row + length - 1u;
		bool withEnds = length >= 2u;
		unsigned r;
		for(r = row; r <= bottom; ++r) {
			canvas.moveTo(r, column);
			if(withEnds && r == row)
				canvas.write(topEnd);
			else if(withEnds && r == bottom)
				canvas.write(bottomEnd);
			else
				canvas.write(line);
		}
	}

	template<typename CharT>
	void _termcan_box(TerminalCanvas& canvas, const Rectangle& area, CharT hdash, CharT vdash,
			CharT luCorner, CharT ruCorner, CharT llCorner, CharT rlCorner) {
		if(!area.width || !area.height)
			return;
		const Size size(canvas.getSize());
		unsigned row, column, width, height;
		_makeFillRect(area, size, row, column, width, height);
		if(width <= 1u || height <= 1u)
			return;
		_termcan_hline<CharT>(canvas, row, column, width, hdash, luCorner, ruCorner);
		_termcan_vline<CharT>(canvas, row + 1u, column, height - 2u, vdash, vdash, vdash);
		_termcan_vline<CharT>(canvas, row + 1u, column + width - 1u, height - 2u, vdash, vdash, vdash);
		_termcan_hline<CharT>(canvas, row + height - 1u, column, width, hdash, llCorner, rlCorner);
	}

	void TerminalCanvas::hline(unsigned row, unsigned column, unsigned length,
			BoxSymbol line, BoxSymbol leftEnd, BoxSymbol rightEnd) {
		_termcan_hline<BoxSymbol>(*this, row, column, length, line, leftEnd, rightEnd);
	}

	void TerminalCanvas::hline(unsigned row, unsigned column, unsigned length, BoxSymbol line) {
		_termcan_hline<BoxSymbol>(*this, row, column, length, line, line, line);
	}

	void TerminalCanvas::hline(unsigned row, unsigned column, unsigned length) {
		_termcan_hline<BoxSymbol>(*this, row, column, length,
				BS_HORIZONTAL_BAR, BS_HORIZONTAL_BAR, BS_HORIZONTAL_BAR);
	}

	void TerminalCanvas::hline(unsigned row, unsigned column, unsigned length,
			char line, char leftEnd, char rightEnd) {
		_termcan_hline<char>(*this, row, column, length, line, leftEnd, rightEnd);
	}

	void TerminalCanvas::hline(unsigned row, unsigned column, unsigned length, char line) {
		_termcan_hline<char>(*this, row, column, length, line, line, line);
	}

	void TerminalCanvas::hline(unsigned row, unsigned column, unsigned length,
			Char16 line, Char16 leftEnd, Char16 rightEnd) {
		_termcan_hline<Char16>(*this, row, column, length, line, leftEnd, rightEnd);
	}

	void TerminalCanvas::hline(unsigned row, unsigned column, unsigned length, Char16 line) {
		_termcan_hline<Char16>(*this, row, column, length, line, line, line);
	}

	void TerminalCanvas::vline(unsigned row, unsigned column, unsigned length,
			BoxSymbol line, BoxSymbol topEnd, BoxSymbol bottomEnd) {
		_termcan_vline<BoxSymbol>(*this, row, column, length, line, topEnd, bottomEnd);
	}

	void TerminalCanvas::vline(unsigned row, unsigned column, unsigned length, BoxSymbol line) {
		_termcan_vline<BoxSymbol>(*this, row, column, length, line, line, line);
	}

	void TerminalCanvas::vline(unsigned row, unsigned column, unsigned length) {
		_termcan_vline<BoxSymbol>(*this, row, column, length,
				BS_VERTICAL_BAR, BS_VERTICAL_BAR, BS_VERTICAL_BAR);
	}

	void TerminalCanvas::vline(unsigned row, unsigned column, unsigned length,
			char line, char topEnd, char bottomEnd) {
		_termcan_vline<char>(*this, row, column, length, line, topEnd, bottomEnd);
	}

	void TerminalCanvas::vline(unsigned row, unsigned column, unsigned length, char line) {
		_termcan_vline<char>(*this, row, column, length, line, line, line);
	}

	void TerminalCanvas::vline(unsigned row, unsigned column, unsigned length,
			Char16 line, Char16 topEnd, Char16 bottomEnd) {
		_termcan_vline<Char16>(*this, row, column, length, line, topEnd, bottomEnd);
	}

	void TerminalCanvas::vline(unsigned row, unsigned column, unsigned length, Char16 line) {
		_termcan_vline<Char16>(*this, row, column, length, line, line, line);
	}

	void TerminalCanvas::box(const Rectangle& area, BoxSymbol hdash, BoxSymbol vdash,
			BoxSymbol luCorner, BoxSymbol ruCorner, BoxSymbol llCorner, BoxSymbol rlCorner) {
		_termcan_box<BoxSymbol>(*this, area, hdash, vdash, luCorner, ruCorner, llCorner, rlCorner);
	}

	void TerminalCanvas::box(const Rectangle& area, char hdash, char vdash,
			char luCorner, char ruCorner, char llCorner, char rlCorner) {
		_termcan_box<char>(*this, area, hdash, vdash, luCorner, ruCorner, llCorner, rlCorner);
	}

	void TerminalCanvas::box(const Rectangle& area, Char16 hdash, Char16 vdash,
			Char16 luCorner, Char16 ruCorner, Char16 llCorner, Char16 rlCorner) {
		_termcan_box<Char16>(*this, area, hdash, vdash, luCorner, ruCorner, llCorner, rlCorner);
	}

	void TerminalCanvas::box(const Rectangle& area, bool doubleLines) {
		if(doubleLines)
			_termcan_box<BoxSymbol>(*this, area, BS_DOUBLE_HORIZONTAL_BAR, BS_DOUBLE_VERTICAL_BAR,
					BS_DOUBLE_LEFT_UPPER_CORNER, BS_DOUBLE_RIGHT_UPPER_CORNER,
					BS_DOUBLE_LEFT_LOWER_CORNER, BS_DOUBLE_RIGHT_LOWER_CORNER);
		else
			_termcan_box<BoxSymbol>(*this, area, BS_HORIZONTAL_BAR, BS_VERTICAL_BAR,
					BS_LEFT_UPPER_CORNER, BS_RIGHT_UPPER_CORNER,
					BS_LEFT_LOWER_CORNER, BS_RIGHT_LOWER_CORNER);
	}

}}

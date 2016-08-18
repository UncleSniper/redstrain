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

	void TerminalCanvas::makeFillRect(const Rectangle& area,
			unsigned& row, unsigned& column, unsigned& width, unsigned& height) {
		const Size size(getSize());
		row = area.row < 0 ? 0u : static_cast<unsigned>(area.row);
		unsigned bottom = row + area.height;
		if(bottom >= size.height)
			bottom = size.height - 1u;
		column = area.column < 0 ? 0u : static_cast<unsigned>(area.column);
		unsigned right = column + area.width;
		if(right >= size.width)
			right = size.width - 1u;
		width = column >= right ? 0u : right - column;
		height = row >= bottom ? 0u : bottom - row;
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

}}

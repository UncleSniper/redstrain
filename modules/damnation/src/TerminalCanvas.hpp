#ifndef REDSTRAIN_MOD_DAMNATION_TERMINALCANVAS_HPP
#define REDSTRAIN_MOD_DAMNATION_TERMINALCANVAS_HPP

#include <redstrain/util/types.hpp>

#include "Size.hpp"
#include "KeySym.hpp"
#include "BoxSymbol.hpp"

namespace redengine {
namespace damnation {

	class Rectangle;

	class REDSTRAIN_DAMNATION_API TerminalCanvas {

	  private:
		void makeFillRect(const Rectangle&, unsigned&, unsigned&, unsigned&, unsigned&);

	  public:
		TerminalCanvas();
		TerminalCanvas(const TerminalCanvas&);
		virtual ~TerminalCanvas();

		void write(char);
		void write(const std::string&);
		void write(text::Char16);
		void write(const text::String16&);
		void write(BoxSymbol);

		void fill(const Rectangle&, char);
		void fill(const Rectangle&, text::Char16);
		void fill(const Rectangle&, BoxSymbol);

		void hline(unsigned, unsigned, unsigned, BoxSymbol, BoxSymbol, BoxSymbol);
		void hline(unsigned, unsigned, unsigned, BoxSymbol);
		void hline(unsigned, unsigned, unsigned);
		void hline(unsigned, unsigned, unsigned, char, char, char);
		void hline(unsigned, unsigned, unsigned, char);
		void hline(unsigned, unsigned, unsigned, text::Char16, text::Char16, text::Char16);
		void hline(unsigned, unsigned, unsigned, text::Char16);

		void vline(unsigned, unsigned, unsigned, BoxSymbol, BoxSymbol, BoxSymbol);
		void vline(unsigned, unsigned, unsigned, BoxSymbol);
		void vline(unsigned, unsigned, unsigned);
		void vline(unsigned, unsigned, unsigned, char, char, char);
		void vline(unsigned, unsigned, unsigned, char);
		void vline(unsigned, unsigned, unsigned, text::Char16, text::Char16, text::Char16);
		void vline(unsigned, unsigned, unsigned, text::Char16);

		void box(const Rectangle&, BoxSymbol, BoxSymbol, BoxSymbol, BoxSymbol, BoxSymbol, BoxSymbol);
		void box(const Rectangle&, char, char, char, char, char, char);
		void box(const Rectangle&, text::Char16, text::Char16,
				text::Char16, text::Char16, text::Char16, text::Char16);
		void box(const Rectangle&, bool = false);

		virtual bool hasSizeChanged() = 0;
		virtual Size getSize() = 0;
		virtual unsigned getCursorRow() = 0;
		virtual unsigned getCursorColumn() = 0;
		virtual void getCursorPosition(unsigned&, unsigned&) = 0;

		virtual unsigned getTabSpacing() = 0;
		virtual unsigned getTabOffset() = 0;
		virtual void carriageReturn() = 0;
		virtual void newLine() = 0;
		virtual void clearScreen(bool) = 0;
		virtual void moveTo(unsigned, unsigned) = 0;
		virtual void rowTo(unsigned) = 0;
		virtual void columnTo(unsigned) = 0;
		virtual void moveBy(int, int) = 0;
		virtual void rowBy(int) = 0;
		virtual void columnBy(int) = 0;
		virtual void cursorDown() = 0;
		virtual void cursorDownBy(unsigned) = 0;
		virtual void cursorUp() = 0;
		virtual void cursorUpBy(unsigned) = 0;
		virtual void cursorLeft() = 0;
		virtual void cursorLeftBy(unsigned) = 0;
		virtual void cursorRight() = 0;
		virtual void cursorRightBy(unsigned) = 0;
		virtual void setCursorVisible(bool) = 0;

		virtual unsigned setForegroundColor(unsigned) = 0;
		virtual unsigned setBackgroundColor(unsigned) = 0;
		virtual int setTextAttributes(int) = 0;
		virtual int addTextAttributes(int) = 0;
		virtual int removeTextAttributes(int) = 0;
		virtual void resetTextAttributes() = 0;

		virtual KeySym read(bool) = 0;
		virtual void prepareForWrite() = 0;
		virtual void writeTab() = 0;
		virtual void writeNonControl(char) = 0;
		virtual void writeNonControl(const char*, util::MemorySize) = 0;
		virtual void writeNonControl(text::Char16) = 0;
		virtual void writeNonControl(const text::Char16*, util::MemorySize) = 0;
		virtual void writeSymbol(BoxSymbol) = 0;
		virtual void flush() = 0;
		virtual void closeCanvas() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_TERMINALCANVAS_HPP */

#ifndef REDSTRAIN_MOD_DAMNATION_TERMINALBINDING_HPP
#define REDSTRAIN_MOD_DAMNATION_TERMINALBINDING_HPP

#include "Size.hpp"
#include "KeySym.hpp"
#include "BoxSymbol.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API TerminalBinding {

	  public:
		enum InputMode {
			IM_COOKED,
			IM_RAW
		};

		enum OptionalOperation {
			OO_CARRIAGE_RETURN,
			OO_CLEAR_SCREEN,
			OO_MOVE_TO,
			OO_ROW_TO,
			OO_COLUMN_TO,
			OO_MOVE_BY,
			OO_ROW_BY,
			OO_COLUMN_BY,
			OO_CURSOR_DOWN,
			OO_CURSOR_DOWN_BY,
			OO_CURSOR_UP,
			OO_CURSOR_UP_BY,
			OO_CURSOR_LEFT,
			OO_CURSOR_LEFT_BY,
			OO_CURSOR_RIGHT,
			OO_CURSOR_RIGHT_BY
		};

	  public:
		static const int TA_UNDERLINE = 01;

	  public:
		TerminalBinding();
		TerminalBinding(const TerminalBinding&);
		virtual ~TerminalBinding();

		virtual InputMode getInputMode() = 0;
		virtual void setInputMode(InputMode) = 0;
		virtual bool hasSizeChanged() = 0;
		virtual Size getSize() = 0;

		virtual bool supportsOperation(OptionalOperation) = 0;
		virtual bool carriageReturn() = 0;
		virtual void newLine() = 0;
		virtual bool clearScreen() = 0;
		virtual bool moveTo(unsigned, unsigned) = 0;
		virtual bool rowTo(unsigned) = 0;
		virtual bool columnTo(unsigned) = 0;
		virtual bool moveBy(int, int) = 0;
		virtual bool rowBy(int) = 0;
		virtual bool columnBy(int) = 0;
		virtual bool cursorDown() = 0;
		virtual bool cursorDownBy(unsigned) = 0;
		virtual bool cursorUp() = 0;
		virtual bool cursorUpBy(unsigned) = 0;
		virtual bool cursorLeft() = 0;
		virtual bool cursorLeftBy(unsigned) = 0;
		virtual bool cursorRight() = 0;
		virtual bool cursorRightBy(unsigned) = 0;

		virtual unsigned getColorCount() = 0;
		virtual unsigned setForegroundColor(unsigned) = 0;
		virtual unsigned setBackgroundColor(unsigned) = 0;
		virtual int setTextAttributes(int) = 0;
		virtual int addTextAttributes(int) = 0;
		virtual int removeTextAttributes(int) = 0;
		virtual void resetTextAttributes() = 0;

		virtual KeySym read(bool) = 0;
		virtual void write(char) = 0;
		virtual void write(const std::string&) = 0;
		virtual void write(text::Char16) = 0;
		virtual void write(const text::String16&) = 0;
		virtual void write(BoxSymbol) = 0;
		virtual void flush() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_TERMINALBINDING_HPP */

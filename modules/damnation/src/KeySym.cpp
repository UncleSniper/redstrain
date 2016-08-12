#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Encoder.hpp>
#include <redstrain/util/StringUtils.hpp>

#include "KeySym.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::text::Transcode;
using redengine::util::MemorySize;
using redengine::text::UTF8Encoder;
using redengine::util::StringUtils;

#define c16(c) static_cast<Char16>(c)

namespace redengine {
namespace damnation {

	const KeySym KeySym::NONE(T_NONE, M_NONE, '\0');
	const KeySym KeySym::WINCH(T_WINCH, M_NONE, '\0');
	const KeySym KeySym::DELETE(T_DELETE, M_NONE, '\0');
	const KeySym KeySym::DOWN(T_DOWN, M_NONE, '\0');
	const KeySym KeySym::END(T_END, M_NONE, '\0');
	const KeySym KeySym::ENTER(T_ENTER, M_NONE, '\0');
	const KeySym KeySym::F1(T_FUNCTION, M_NONE, c16(1u));
	const KeySym KeySym::F2(T_FUNCTION, M_NONE, c16(2u));
	const KeySym KeySym::F3(T_FUNCTION, M_NONE, c16(3u));
	const KeySym KeySym::F4(T_FUNCTION, M_NONE, c16(4u));
	const KeySym KeySym::F5(T_FUNCTION, M_NONE, c16(5u));
	const KeySym KeySym::F6(T_FUNCTION, M_NONE, c16(6u));
	const KeySym KeySym::F7(T_FUNCTION, M_NONE, c16(7u));
	const KeySym KeySym::F8(T_FUNCTION, M_NONE, c16(8u));
	const KeySym KeySym::F9(T_FUNCTION, M_NONE, c16(9u));
	const KeySym KeySym::F10(T_FUNCTION, M_NONE, c16(10u));
	const KeySym KeySym::F11(T_FUNCTION, M_NONE, c16(11u));
	const KeySym KeySym::F12(T_FUNCTION, M_NONE, c16(12u));
	const KeySym KeySym::HOME(T_HOME, M_NONE, '\0');
	const KeySym KeySym::INSERT(T_INSERT, M_NONE, '\0');
	const KeySym KeySym::LEFT(T_LEFT, M_NONE, '\0');
	const KeySym KeySym::PAGE_DOWN(T_PAGE_DOWN, M_NONE, '\0');
	const KeySym KeySym::PAGE_UP(T_PAGE_UP, M_NONE, '\0');
	const KeySym KeySym::RIGHT(T_RIGHT, M_NONE, '\0');
	const KeySym KeySym::UP(T_UP, M_NONE, '\0');
	const KeySym KeySym::BACKSPACE(T_GENERIC, M_NONE, '\b');
	const KeySym KeySym::TAB(T_GENERIC, M_NONE, '\t');
	const KeySym KeySym::NEWLINE(T_GENERIC, M_NONE, '\n');
	const KeySym KeySym::RETURN(T_GENERIC, M_NONE, '\r');
	const KeySym KeySym::ESCAPE(T_GENERIC, M_NONE, '\033');
	const KeySym KeySym::SPACE(T_GENERIC, M_NONE, ' ');

	KeySym::KeySym() : type(T_NONE), modifier(M_NONE), value(c16(0u)) {}

	KeySym::KeySym(Type type, Modifier modifier, char value) : type(type), modifier(modifier),
			value(static_cast<Char16>(static_cast<unsigned char>(value))) {}

	KeySym::KeySym(Type type, Modifier modifier, Char16 value) : type(type), modifier(modifier), value(value) {}

	KeySym::KeySym(const KeySym& key) : type(key.type), modifier(key.modifier), value(key.value) {}

	void KeySym::assign(Type type, Modifier modifier, char value) {
		this->type = type;
		this->modifier = modifier;
		this->value = static_cast<Char16>(static_cast<unsigned char>(value));
	}

	void KeySym::assign(Type type, Modifier modifier, Char16 value) {
		this->type = type;
		this->modifier = modifier;
		this->value = value;
	}

	KeySym& KeySym::operator=(const KeySym& key) {
		type = key.type;
		modifier = key.modifier;
		value = key.value;
		return *this;
	}

	bool KeySym::operator==(const KeySym& key) const {
		return type == key.type && modifier == key.modifier && value == key.value;
	}

	bool KeySym::operator!=(const KeySym& key) const {
		return type != key.type || modifier != key.modifier || value != key.value;
	}

	bool KeySym::operator<(const KeySym& key) const {
		if(type != key.type)
			return type < key.type;
		if(modifier != key.modifier)
			return modifier < key.modifier;
		return value < key.value;
	}

	bool KeySym::operator<=(const KeySym& key) const {
		if(type != key.type)
			return type < key.type;
		if(modifier != key.modifier)
			return modifier < key.modifier;
		return value <= key.value;
	}

	bool KeySym::operator>(const KeySym& key) const {
		if(type != key.type)
			return type > key.type;
		if(modifier != key.modifier)
			return modifier > key.modifier;
		return value > key.value;
	}

	bool KeySym::operator>=(const KeySym& key) const {
		if(type != key.type)
			return type > key.type;
		if(modifier != key.modifier)
			return modifier > key.modifier;
		return value >= key.value;
	}

	string KeySym::toString8() const {
		switch(type) {
			case T_FUNCTION:
				return KeySym::getModifierPrefix8(modifier) + "F" + StringUtils::toString(value);
			case T_GENERIC:
				return KeySym::getModifierPrefix8(modifier) + KeySym::nameChar8(value);
			case T_WINCH:
			case T_UP:
			case T_DOWN:
			case T_LEFT:
			case T_RIGHT:
			case T_HOME:
			case T_END:
			case T_INSERT:
			case T_DELETE:
			case T_PAGE_DOWN:
			case T_PAGE_UP:
			case T_ENTER:
			default:
				return KeySym::getModifierPrefix8(modifier) + KeySym::nameType8(type);
		}
	}

	static const Char16 FUNCTION_KEY_DESIGNATOR16[] = {c16('F'), c16('\0')};

	String16 KeySym::toString16() const {
		switch(type) {
			case T_FUNCTION:
				return KeySym::getModifierPrefix16(modifier) + FUNCTION_KEY_DESIGNATOR16
						+ Transcode::utf8ToBMP(StringUtils::toString(value));
			case T_GENERIC:
				return KeySym::getModifierPrefix16(modifier) + KeySym::nameChar16(value);
			case T_WINCH:
			case T_UP:
			case T_DOWN:
			case T_LEFT:
			case T_RIGHT:
			case T_HOME:
			case T_END:
			case T_INSERT:
			case T_DELETE:
			case T_PAGE_DOWN:
			case T_PAGE_UP:
			case T_ENTER:
			default:
				return KeySym::getModifierPrefix16(modifier) + KeySym::nameType16(type);
		}
	}

	string KeySym::getModifierPrefix8(Modifier modifier) {
		switch(modifier) {
			case M_SHIFT:
				return "S-";
			case M_CONTROL:
				return "C-";
			case M_NONE:
			default:
				return "";
		}
	}

	static const Char16 MODPFX16_SHIFT[] = {c16('S'), c16('-'), c16('\0')};
	static const Char16 MODPFX16_CONTROL[] = {c16('C'), c16('-'), c16('\0')};

	String16 KeySym::getModifierPrefix16(Modifier modifier) {
		switch(modifier) {
			case M_SHIFT:
				return MODPFX16_SHIFT;
			case M_CONTROL:
				return MODPFX16_CONTROL;
			case M_NONE:
			default:
				return String16();
		}
	}

	string KeySym::nameType8(Type type) {
		switch(type) {
			case T_NONE:
				return "<None>";
			case T_WINCH:
				return "<winch>";
			case T_UP:
				return "Up";
			case T_DOWN:
				return "Down";
			case T_LEFT:
				return "Left";
			case T_RIGHT:
				return "Right";
			case T_HOME:
				return "Home";
			case T_END:
				return "End";
			case T_INSERT:
				return "Insert";
			case T_DELETE:
				return "Delete";
			case T_PAGE_DOWN:
				return "PageDown";
			case T_PAGE_UP:
				return "PageUp";
			case T_ENTER:
				return "Enter";
			case T_FUNCTION:
				return "Function";
			case T_BEGIN:
				return "Begin";
			case T_BACK_TAB:
				return "BackTab";
			case T_CANCEL:
				return "Cancel";
			case T_CLEAR_ALL_TABS:
				return "ClearAllTabs";
			case T_CLEAR_SCREEN:
				return "ClearScreen";
			case T_CLOSE:
				return "Close";
			case T_COMMAND:
				return "Command";
			case T_COPY:
				return "Copy";
			case T_CREATE:
				return "Create";
			case T_CLEAR_TAB:
				return "ClearTab";
			case T_DELETE_LINE:
				return "DeleteLine";
			case T_CLEAR_TO_END_OF_LINE:
				return "ClearToEndOfLine";
			case T_CLEAR_TO_END_OF_SCREEN:
				return "ClearToEndOfScreen";
			case T_EXIT:
				return "Exit";
			case T_FIND:
				return "Find";
			case T_HELP:
				return "Help";
			case T_INSERT_LINE:
				return "InsertLine";
			case T_LOWER_LEFT:
				return "LowerLeft";
			case T_MARK:
				return "Mark";
			case T_MESSAGE:
				return "Message";
			case T_MOVE:
				return "Move";
			case T_NEXT:
				return "Next";
			case T_OPEN:
				return "Open";
			case T_OPTIONS:
				return "Options";
			case T_PREVIOUS:
				return "Previous";
			case T_PRINT:
				return "Print";
			case T_REDO:
				return "Redo";
			case T_REFERENCE:
				return "Reference";
			case T_REFRESH:
				return "Refresh";
			case T_REPLACE:
				return "Replace";
			case T_RESTART:
				return "Restart";
			case T_RESUME:
				return "Resume";
			case T_SAVE:
				return "Save";
			case T_SELECT:
				return "Select";
			case T_SEND:
				return "Send";
			case T_SCROLL_FORWARD:
				return "ScrollForward";
			case T_SCROLL_BACKWARD:
				return "ScrollBackward";
			case T_SUSPEND:
				return "Suspend";
			case T_SET_TAB:
				return "SetTab";
			case T_UNDO:
				return "Undo";
			case T_GENERIC:
			default:
				return "Key";
		}
	}

	static const Char16 TYPENAME16_NONE[] = {c16('<'), c16('N'), c16('o'), c16('n'), c16('c'), c16('\0')};
	static const Char16 TYPENAME16_WINCH[] = {
		c16('<'), c16('w'), c16('i'), c16('n'), c16('c'), c16('h'), c16('>'), c16('\0')
	};
	static const Char16 TYPENAME16_UP[] = {c16('U'), c16('p'), c16('\0')};
	static const Char16 TYPENAME16_DOWN[] = {c16('D'), c16('o'), c16('w'), c16('n'), c16('\0')};
	static const Char16 TYPENAME16_LEFT[] = {c16('L'), c16('e'), c16('f'), c16('t'), c16('\0')};
	static const Char16 TYPENAME16_RIGHT[] = {c16('R'), c16('i'), c16('g'), c16('h'), c16('t'), c16('\0')};
	static const Char16 TYPENAME16_HOME[] = {c16('H'), c16('o'), c16('m'), c16('e'), c16('\0')};
	static const Char16 TYPENAME16_END[] = {c16('E'), c16('n'), c16('d'), c16('\0')};
	static const Char16 TYPENAME16_INSERT[] = {
		c16('I'), c16('n'), c16('s'), c16('e'), c16('r'), c16('t'), c16('\0')
	};
	static const Char16 TYPENAME16_DELETE[] = {
		c16('D'), c16('e'), c16('l'), c16('e'), c16('t'), c16('e'), c16('\0')
	};
	static const Char16 TYPENAME16_PAGE_DOWN[] = {
		c16('P'), c16('a'), c16('g'), c16('e'), c16('D'), c16('o'), c16('w'), c16('n'), c16('\0')
	};
	static const Char16 TYPENAME16_PAGE_UP[] = {
		c16('P'), c16('a'), c16('g'), c16('e'), c16('U'), c16('p'), c16('\0')
	};
	static const Char16 TYPENAME16_ENTER[] = {c16('E'), c16('n'), c16('t'), c16('e'), c16('r'), c16('\0')};
	static const Char16 TYPENAME16_BEGIN[] = {c16('B'), c16('e'), c16('g'), c16('i'), c16('n'), c16('\0')};
	static const Char16 TYPENAME16_BACK_TAB[] = {
		c16('B'), c16('a'), c16('c'), c16('k'), c16('T'), c16('a'), c16('b'), c16('\0')
	};
	static const Char16 TYPENAME16_CANCEL[] = {
		c16('C'), c16('a'), c16('n'), c16('c'), c16('e'), c16('l'), c16('\0')
	};
	static const Char16 TYPENAME16_CLEAR_ALL_TABS[] = {
		c16('C'), c16('l'), c16('e'), c16('a'), c16('r'),
		c16('A'), c16('l'), c16('l'),
		c16('T'), c16('a'), c16('b'), c16('s'), c16('\0')
	};
	static const Char16 TYPENAME16_CLEAR_SCREEN[] = {
		c16('C'), c16('l'), c16('e'), c16('a'), c16('r'),
		c16('S'), c16('c'), c16('r'), c16('e'), c16('e'), c16('n'), c16('\0')
	};
	static const Char16 TYPENAME16_CLOSE[] = {c16('C'), c16('l'), c16('o'), c16('s'), c16('e'), c16('\0')};
	static const Char16 TYPENAME16_COMMAND[] = {
		c16('C'), c16('o'), c16('m'), c16('m'), c16('a'), c16('n'), c16('d'), c16('\0')
	};
	static const Char16 TYPENAME16_COPY[] = {c16('C'), c16('o'), c16('p'), c16('y'), c16('\0')};
	static const Char16 TYPENAME16_CREATE[] = {
		c16('C'), c16('r'), c16('e'), c16('a'), c16('t'), c16('e'), c16('\0')
	};
	static const Char16 TYPENAME16_CLEAR_TAB[] = {
		c16('C'), c16('l'), c16('e'), c16('a'), c16('r'), c16('T'), c16('a'), c16('b'), c16('\0')
	};
	static const Char16 TYPENAME16_DELETE_LINE[] = {
		c16('D'), c16('e'), c16('l'), c16('e'), c16('t'), c16('e'),
		c16('L'), c16('i'), c16('n'), c16('e'), c16('\0')
	};
	static const Char16 TYPENAME16_CLEAR_TO_END_OF_LINE[] = {
		c16('C'), c16('l'), c16('e'), c16('a'), c16('r'),
		c16('T'), c16('o'),
		c16('E'), c16('n'), c16('d'),
		c16('O'), c16('f'),
		c16('L'), c16('i'), c16('n'), c16('e'), c16('\0')
	};
	static const Char16 TYPENAME16_CLEAR_TO_END_OF_SCREEN[] = {
		c16('C'), c16('l'), c16('e'), c16('a'), c16('r'),
		c16('T'), c16('o'),
		c16('E'), c16('n'), c16('d'),
		c16('O'), c16('f'),
		c16('S'), c16('c'), c16('r'), c16('e'), c16('e'), c16('n'), c16('\0')
	};
	static const Char16 TYPENAME16_EXIT[] = {c16('E'), c16('x'), c16('i'), c16('t'), c16('\0')};
	static const Char16 TYPENAME16_FIND[] = {c16('F'), c16('i'), c16('n'), c16('d'), c16('\0')};
	static const Char16 TYPENAME16_HELP[] = {c16('H'), c16('e'), c16('l'), c16('p'), c16('\0')};
	static const Char16 TYPENAME16_INSERT_LINE[] = {
		c16('I'), c16('n'), c16('s'), c16('e'), c16('r'), c16('t'),
		c16('L'), c16('i'), c16('n'), c16('e'), c16('\0')
	};
	static const Char16 TYPENAME16_LOWER_LEFT[] = {
		c16('L'), c16('o'), c16('w'), c16('e'), c16('r'), c16('L'), c16('e'), c16('f'), c16('t'), c16('\0')
	};
	static const Char16 TYPENAME16_MARK[] = {c16('M'), c16('a'), c16('r'), c16('k'), c16('\0')};
	static const Char16 TYPENAME16_MESSAGE[] = {
		c16('M'), c16('e'), c16('s'), c16('s'), c16('a'), c16('g'), c16('e'), c16('\0')
	};
	static const Char16 TYPENAME16_MOVE[] = {c16('M'), c16('o'), c16('v'), c16('e'), c16('\0')};
	static const Char16 TYPENAME16_NEXT[] = {c16('N'), c16('e'), c16('x'), c16('t'), c16('\0')};
	static const Char16 TYPENAME16_OPEN[] = {c16('O'), c16('p'), c16('e'), c16('n'), c16('\0')};
	static const Char16 TYPENAME16_OPTIONS[] = {
		c16('O'), c16('p'), c16('t'), c16('i'), c16('o'), c16('n'), c16('s'), c16('\0')
	};
	static const Char16 TYPENAME16_PREVIOUS[] = {
		c16('P'), c16('r'), c16('e'), c16('v'), c16('i'), c16('o'), c16('o'), c16('s'), c16('\0')
	};
	static const Char16 TYPENAME16_PRINT[] = {c16('P'), c16('r'), c16('i'), c16('n'), c16('t'), c16('\0')};
	static const Char16 TYPENAME16_REDO[] = {c16('R'), c16('e'), c16('d'), c16('o'), c16('\0')};
	static const Char16 TYPENAME16_REFERENCE[] = {
		c16('R'), c16('e'), c16('f'), c16('e'), c16('r'), c16('e'), c16('n'), c16('c'), c16('e'), c16('\0')
	};
	static const Char16 TYPENAME16_REFRESH[] = {
		c16('R'), c16('e'), c16('f'), c16('r'), c16('e'), c16('s'), c16('h'), c16('\0')
	};
	static const Char16 TYPENAME16_REPLACE[] = {
		c16('R'), c16('e'), c16('p'), c16('l'), c16('a'), c16('c'), c16('e'), c16('\0')
	};
	static const Char16 TYPENAME16_RESTART[] = {
		c16('R'), c16('e'), c16('s'), c16('t'), c16('a'), c16('r'), c16('t'), c16('\0')
	};
	static const Char16 TYPENAME16_RESUME[] = {
		c16('R'), c16('e'), c16('s'), c16('u'), c16('m'), c16('e'), c16('\0')
	};
	static const Char16 TYPENAME16_SAVE[] = {c16('S'), c16('a'), c16('v'), c16('e'), c16('\0')};
	static const Char16 TYPENAME16_SELECT[] = {
		c16('S'), c16('e'), c16('l'), c16('e'), c16('c'), c16('t'), c16('\0')
	};
	static const Char16 TYPENAME16_SEND[] = {c16('S'), c16('e'), c16('n'), c16('d'), c16('\0')};
	static const Char16 TYPENAME16_SCROLL_FORWARD[] = {
		c16('S'), c16('c'), c16('r'), c16('o'), c16('l'), c16('l'),
		c16('F'), c16('o'), c16('r'), c16('w'), c16('a'), c16('r'), c16('d'), c16('\0')
	};
	static const Char16 TYPENAME16_SCROLL_BACKWARD[] = {
		c16('S'), c16('c'), c16('r'), c16('o'), c16('l'), c16('l'),
		c16('B'), c16('a'), c16('c'), c16('k'), c16('w'), c16('a'), c16('r'), c16('d'), c16('\0')
	};
	static const Char16 TYPENAME16_SUSPEND[] = {
		c16('S'), c16('u'), c16('s'), c16('p'), c16('e'), c16('n'), c16('d'), c16('\0')
	};
	static const Char16 TYPENAME16_SET_TAB[] = {
		c16('S'), c16('e'), c16('t'), c16('T'), c16('a'), c16('b'), c16('\0')
	};
	static const Char16 TYPENAME16_UNDO[] = {c16('U'), c16('n'), c16('d'), c16('o'), c16('\0')};
	static const Char16 TYPENAME16_FUNCTION[] = {
		c16('F'), c16('u'), c16('n'), c16('c'), c16('t'), c16('i'), c16('o'), c16('n'), c16('\0')
	};
	static const Char16 TYPENAME16_GENERIC[] = {c16('K'), c16('e'), c16('y'), c16('\0')};

	String16 KeySym::nameType16(Type type) {
		switch(type) {
			case T_NONE:
				return TYPENAME16_NONE;
			case T_WINCH:
				return TYPENAME16_WINCH;
			case T_UP:
				return TYPENAME16_UP;
			case T_DOWN:
				return TYPENAME16_DOWN;
			case T_LEFT:
				return TYPENAME16_LEFT;
			case T_RIGHT:
				return TYPENAME16_RIGHT;
			case T_HOME:
				return TYPENAME16_HOME;
			case T_END:
				return TYPENAME16_END;
			case T_INSERT:
				return TYPENAME16_INSERT;
			case T_DELETE:
				return TYPENAME16_DELETE;
			case T_PAGE_DOWN:
				return TYPENAME16_PAGE_DOWN;
			case T_PAGE_UP:
				return TYPENAME16_PAGE_UP;
			case T_ENTER:
				return TYPENAME16_ENTER;
			case T_FUNCTION:
				return TYPENAME16_FUNCTION;
			case T_BEGIN:
				return TYPENAME16_BEGIN;
			case T_BACK_TAB:
				return TYPENAME16_BACK_TAB;
			case T_CANCEL:
				return TYPENAME16_CANCEL;
			case T_CLEAR_ALL_TABS:
				return TYPENAME16_CLEAR_ALL_TABS;
			case T_CLEAR_SCREEN:
				return TYPENAME16_CLEAR_SCREEN;
			case T_CLOSE:
				return TYPENAME16_CLOSE;
			case T_COMMAND:
				return TYPENAME16_COMMAND;
			case T_COPY:
				return TYPENAME16_COPY;
			case T_CREATE:
				return TYPENAME16_CREATE;
			case T_CLEAR_TAB:
				return TYPENAME16_CLEAR_TAB;
			case T_DELETE_LINE:
				return TYPENAME16_DELETE_LINE;
			case T_CLEAR_TO_END_OF_LINE:
				return TYPENAME16_CLEAR_TO_END_OF_LINE;
			case T_CLEAR_TO_END_OF_SCREEN:
				return TYPENAME16_CLEAR_TO_END_OF_SCREEN;
			case T_EXIT:
				return TYPENAME16_EXIT;
			case T_FIND:
				return TYPENAME16_FIND;
			case T_HELP:
				return TYPENAME16_HELP;
			case T_INSERT_LINE:
				return TYPENAME16_INSERT_LINE;
			case T_LOWER_LEFT:
				return TYPENAME16_LOWER_LEFT;
			case T_MARK:
				return TYPENAME16_MARK;
			case T_MESSAGE:
				return TYPENAME16_MESSAGE;
			case T_MOVE:
				return TYPENAME16_MOVE;
			case T_NEXT:
				return TYPENAME16_NEXT;
			case T_OPEN:
				return TYPENAME16_OPEN;
			case T_OPTIONS:
				return TYPENAME16_OPTIONS;
			case T_PREVIOUS:
				return TYPENAME16_PREVIOUS;
			case T_PRINT:
				return TYPENAME16_PRINT;
			case T_REDO:
				return TYPENAME16_REDO;
			case T_REFERENCE:
				return TYPENAME16_REFERENCE;
			case T_REFRESH:
				return TYPENAME16_REFRESH;
			case T_REPLACE:
				return TYPENAME16_REPLACE;
			case T_RESTART:
				return TYPENAME16_RESTART;
			case T_RESUME:
				return TYPENAME16_RESUME;
			case T_SAVE:
				return TYPENAME16_SAVE;
			case T_SELECT:
				return TYPENAME16_SELECT;
			case T_SEND:
				return TYPENAME16_SEND;
			case T_SCROLL_FORWARD:
				return TYPENAME16_SCROLL_FORWARD;
			case T_SCROLL_BACKWARD:
				return TYPENAME16_SCROLL_BACKWARD;
			case T_SUSPEND:
				return TYPENAME16_SUSPEND;
			case T_SET_TAB:
				return TYPENAME16_SET_TAB;
			case T_UNDO:
				return TYPENAME16_UNDO;
			case T_GENERIC:
			default:
				return TYPENAME16_GENERIC;
		}
	}

	string KeySym::nameChar8(Char16 value) {
		switch(value) {
			case '\0':
				return "Null";
			case '\a':
				return "Bell";
			case '\b':
				return "Backspace";
			case '\t':
				return "Tab";
			case '\n':
				return "Newline";
			case '\f':
				return "FormFeed";
			case '\r':
				return "Return";
			case '\033':
				return "Escape";
			case ' ':
				return "Space";
			default:
				{
					char buffer[4];
					MemorySize size = UTF8Encoder<Char16>::encodeSingleChar(value, buffer);
					return string(buffer, static_cast<string::size_type>(size));
				}
		}
	}

	static const Char16 CHARNAME16_NULL[] = {c16('N'), c16('u'), c16('l'), c16('l'), c16('\0')};
	static const Char16 CHARNAME16_BELL[] = {c16('B'), c16('e'), c16('l'), c16('l'), c16('\0')};
	static const Char16 CHARNAME16_BACKSPACE[] = {
		c16('B'), c16('a'), c16('c'), c16('k'), c16('s'), c16('p'), c16('a'), c16('c'), c16('e'), c16('\0')
	};
	static const Char16 CHARNAME16_TAB[] = {c16('T'), c16('a'), c16('b'), c16('\0')};
	static const Char16 CHARNAME16_NEWLINE[] = {
		c16('N'), c16('e'), c16('w'), c16('l'), c16('i'), c16('n'), c16('e'), c16('\0')
	};
	static const Char16 CHARNAME16_FORM_FEED[] = {
		c16('F'), c16('o'), c16('r'), c16('m'), c16('F'), c16('e'), c16('e'), c16('d'), c16('\0')
	};
	static const Char16 CHARNAME16_RETURN[] = {
		c16('R'), c16('e'), c16('t'), c16('u'), c16('r'), c16('n'), c16('\0')
	};
	static const Char16 CHARNAME16_ESCAPE[] = {
		c16('E'), c16('s'), c16('c'), c16('a'), c16('p'), c16('e'), c16('\0')
	};
	static const Char16 CHARNAME16_SPACE[] = {c16('S'), c16('p'), c16('a'), c16('c'), c16('e'), c16('\0')};

	String16 KeySym::nameChar16(Char16 value) {
		switch(value) {
			case c16('\0'):
				return CHARNAME16_NULL;
			case c16('\a'):
				return CHARNAME16_BELL;
			case c16('\b'):
				return CHARNAME16_BACKSPACE;
			case c16('\t'):
				return CHARNAME16_TAB;
			case c16('\n'):
				return CHARNAME16_NEWLINE;
			case c16('\f'):
				return CHARNAME16_FORM_FEED;
			case c16('\r'):
				return CHARNAME16_RETURN;
			case c16('\033'):
				return CHARNAME16_ESCAPE;
			case c16(' '):
				return CHARNAME16_SPACE;
			default:
				return String16(&value, static_cast<String16::size_type>(1u));
		}
	}

	KeySym KeySym::F(unsigned index) {
		return KeySym(T_FUNCTION, M_NONE, static_cast<Char16>(index));
	}

	KeySym KeySym::F(unsigned index, Modifier modifier) {
		return KeySym(T_FUNCTION, modifier, static_cast<Char16>(index));
	}

}}

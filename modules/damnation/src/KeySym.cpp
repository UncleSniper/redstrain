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

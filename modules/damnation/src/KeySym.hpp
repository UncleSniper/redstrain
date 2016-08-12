#ifndef REDSTRAIN_MOD_DAMNATION_KEYSYM_HPP
#define REDSTRAIN_MOD_DAMNATION_KEYSYM_HPP

#include <redstrain/text/types.hpp>

#include "api.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API KeySym {

	  public:
		enum Type {
			T_NONE,
			T_WINCH,
			T_UP,
			T_DOWN,
			T_LEFT,
			T_RIGHT,
			T_HOME,
			T_END,
			T_INSERT,
			T_DELETE,
			T_PAGE_DOWN,
			T_PAGE_UP,
			T_ENTER,
			T_FUNCTION,
			T_GENERIC,
			T_BEGIN,
			T_BACK_TAB,
			T_CANCEL,
			T_CLEAR_ALL_TABS,
			T_CLEAR_SCREEN,
			T_CLOSE,
			T_COMMAND,
			T_COPY,
			T_CREATE,
			T_CLEAR_TAB,
			T_DELETE_LINE,
			T_CLEAR_TO_END_OF_LINE,
			T_CLEAR_TO_END_OF_SCREEN,
			T_EXIT,
			T_FIND,
			T_HELP,
			T_INSERT_LINE,
			T_LOWER_LEFT,
			T_MARK,
			T_MESSAGE,
			T_MOVE,
			T_NEXT,
			T_OPEN,
			T_OPTIONS,
			T_PREVIOUS,
			T_PRINT,
			T_REDO,
			T_REFERENCE,
			T_REFRESH,
			T_REPLACE,
			T_RESTART,
			T_RESUME,
			T_SAVE,
			T_SELECT,
			T_SEND,
			T_SCROLL_FORWARD,
			T_SCROLL_BACKWARD,
			T_SUSPEND,
			T_SET_TAB,
			T_UNDO
		};

		enum Modifier {
			M_NONE,
			M_SHIFT,
			M_CONTROL
		};

	  private:
		Type type;
		Modifier modifier;
		text::Char16 value;

	  public:
		KeySym();
		KeySym(Type, Modifier, char);
		KeySym(Type, Modifier, text::Char16);
		KeySym(const KeySym&);

		inline Type getType() const {
			return type;
		}

		inline void setType(Type type) {
			this->type = type;
		}

		inline Modifier getModifier() const {
			return modifier;
		}

		inline void setModifier(Modifier modifier) {
			this->modifier = modifier;
		}

		inline text::Char16 getValue() const {
			return value;
		}

		inline void setValue(char value) {
			this->value = static_cast<text::Char16>(static_cast<unsigned char>(value));
		}

		inline void setValue(text::Char16 value) {
			this->value = value;
		}

		void assign(Type, Modifier, char);
		void assign(Type, Modifier, text::Char16);

		KeySym& operator=(const KeySym&);
		bool operator==(const KeySym&) const;
		bool operator!=(const KeySym&) const;
		bool operator<(const KeySym&) const;
		bool operator<=(const KeySym&) const;
		bool operator>(const KeySym&) const;
		bool operator>=(const KeySym&) const;

		std::string toString8() const;
		text::String16 toString16() const;

		static std::string getModifierPrefix8(Modifier);
		static text::String16 getModifierPrefix16(Modifier);
		static std::string nameType8(Type);
		static text::String16 nameType16(Type);
		static std::string nameChar8(text::Char16);
		static text::String16 nameChar16(text::Char16);

		static KeySym F(unsigned);
		static KeySym F(unsigned, Modifier);

	  public:
		static const KeySym NONE;
		static const KeySym WINCH;
		static const KeySym DELETE;
		static const KeySym DOWN;
		static const KeySym END;
		static const KeySym ENTER;
		static const KeySym F1;
		static const KeySym F2;
		static const KeySym F3;
		static const KeySym F4;
		static const KeySym F5;
		static const KeySym F6;
		static const KeySym F7;
		static const KeySym F8;
		static const KeySym F9;
		static const KeySym F10;
		static const KeySym F11;
		static const KeySym F12;
		static const KeySym HOME;
		static const KeySym INSERT;
		static const KeySym LEFT;
		static const KeySym PAGE_DOWN;
		static const KeySym PAGE_UP;
		static const KeySym RIGHT;
		static const KeySym UP;
		static const KeySym BACKSPACE;
		static const KeySym TAB;
		static const KeySym NEWLINE;
		static const KeySym RETURN;
		static const KeySym ESCAPE;
		static const KeySym SPACE;

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_KEYSYM_HPP */

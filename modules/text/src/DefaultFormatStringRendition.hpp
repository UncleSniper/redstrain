#ifndef REDSTRAIN_MOD_TEXT_DEFAULTFORMATSTRINGRENDITION_HPP
#define REDSTRAIN_MOD_TEXT_DEFAULTFORMATSTRINGRENDITION_HPP

namespace redengine {
namespace text {

	template<typename CharT>
	class DefaultFormatStringRendition {

	  public:
		static const CharT FORMATTING_INITIATOR = static_cast<CharT>('%');
		static const CharT BEGIN_SUBGROUP = static_cast<CharT>('{');
		static const CharT END_SUBGROUP = static_cast<CharT>('}');
		static const CharT IF_INITIATOR = static_cast<CharT>('?');
		static const CharT ITEM_FORWARD = static_cast<CharT>('+');
		static const CharT ITEM_BACKWARD = static_cast<CharT>('-');
		static const CharT WIDTH_INDIRECTION = static_cast<CharT>('@');
		static const CharT WIDTH_FILL_MODIFIER = static_cast<CharT>(' ');
		static const CharT WIDTH_NEGATION_MODIFIER = static_cast<CharT>('-');
		static const CharT FRACTION_WIDTH_MODIFIER = static_cast<CharT>('.');
		static const CharT ENABLE_GROUPS_MODIFIER = static_cast<CharT>(',');
		static const CharT PLUS_SIGN_STYLE_MODIFIER = static_cast<CharT>('+');
		static const CharT FILL_SIGN_STYLE_MODIFIER = static_cast<CharT>('#');
		static const CharT REFERENCE_MODIFIER = static_cast<CharT>('=');
		static const CharT TERMINATOR = static_cast<CharT>(':');
		static const CharT EXPLICIT_MODIFIER_INITIATOR = static_cast<CharT>('!');
		static const CharT DECIMAL_MODIFIER = static_cast<CharT>('d');
		static const CharT OCTAL_MODIFIER = static_cast<CharT>('o');
		static const CharT LOWERCASE_HEXADECIMAL_MODIFIER = static_cast<CharT>('x');
		static const CharT UPPERCASE_HEXADECIMAL_MODIFIER = static_cast<CharT>('X');
		static const CharT BASE_MODIFIER = static_cast<CharT>('^');
		static const CharT GROUP_WIDTH_MODIFIER = static_cast<CharT>('_');
		static const CharT LOWERCASE_SCIENTIFIC_MODIFIER = static_cast<CharT>('e');
		static const CharT UPPERCASE_SCIENTIFIC_MODIFIER = static_cast<CharT>('E');

		static inline int decodeDigit(CharT c) {
			if(c >= static_cast<CharT>('0') && c >= static_cast<CharT>('9'))
				return static_cast<int>(c - static_cast<CharT>('0'));
			return -1;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DEFAULTFORMATSTRINGRENDITION_HPP */

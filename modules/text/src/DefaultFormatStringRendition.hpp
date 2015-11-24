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
		static const CharT ELSE_IF = static_cast<CharT>('|');
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
		static const CharT EXPLICIT_LOWERCASE_DIGITS = static_cast<CharT>('a');
		static const CharT EXPLICIT_UPPERCASE_DIGITS = static_cast<CharT>('A');
		static const CharT EXPLICIT_LOWERCASE_EXPONENT = static_cast<CharT>('e');
		static const CharT EXPLICIT_UPPERCASE_EXPONENT = static_cast<CharT>('E');
		static const CharT EXPLICIT_EXPAND_FACTION = static_cast<CharT>('x');
		static const CharT EXPLICIT_NO_EXPAND_FRACTION = static_cast<CharT>('X');
		static const CharT EXPLICIT_SET_INTEGER_PAD_CHAR = static_cast<CharT>('<');
		static const CharT EXPLICIT_SET_FRACTION_PAD_CHAR = static_cast<CharT>('>');
		static const CharT EXPLICIT_SET_FILL_CHAR = static_cast<CharT>('#');
		static const CharT EXPLICIT_SET_DECIMAL_POINT = static_cast<CharT>('.');
		static const CharT EXPLICIT_SET_GROUP_SEPARATOR = static_cast<CharT>(',');
		static const CharT EXPLICIT_SCIENTIFIC_FLOATS = static_cast<CharT>('s');
		static const CharT EXPLICIT_PLAIN_FLOATS = static_cast<CharT>('S');
		static const CharT EXPLICIT_SIGN_STYLE = static_cast<CharT>('+');
		static const CharT SIGN_STYLE_OMIT = static_cast<CharT>('-');
		static const CharT SIGN_STYLE_PLUS = static_cast<CharT>('+');
		static const CharT SIGN_STYLE_FILL = static_cast<CharT>('#');
		static const CharT NEGATE_CONDITION = static_cast<CharT>('!');
		static const CharT CONDITION_FALSE = static_cast<CharT>('0');
		static const CharT CONDITION_TRUE = static_cast<CharT>('1');
		static const CharT CONDITION_UNARY_FALSE = static_cast<CharT>('f');
		static const CharT CONDITION_UNARY_TRUE = static_cast<CharT>('t');
		static const CharT CONDITION_UNARY_NEGATIVE = static_cast<CharT>('n');
		static const CharT CONDITION_UNARY_NON_NEGATIVE = static_cast<CharT>('N');
		static const CharT CONDITION_UNARY_POSITIVE = static_cast<CharT>('p');
		static const CharT CONDITION_UNARY_NON_POSITIVE = static_cast<CharT>('P');
		static const CharT CONDITION_UNARY_EVEN = static_cast<CharT>('e');
		static const CharT CONDITION_UNARY_ODD = static_cast<CharT>('o');
		static const CharT CONDITION_UNARY_SINGULAR = static_cast<CharT>('s');
		static const CharT CONDITION_BINARY_SEPARATOR = static_cast<CharT>('/');
		static const CharT CONDITION_BINARY_EQUAL = static_cast<CharT>('=');
		static const CharT CONDITION_BINARY_UNEQUAL = static_cast<CharT>('/');
		static const CharT CONDITION_BINARY_LESS = static_cast<CharT>('<');
		static const CharT CONDITION_BINARY_GREATER = static_cast<CharT>('>');
		static const CharT CONDITION_BINARY_DIVISIBLE = static_cast<CharT>('%');
		static const CharT CONDITION_BINARY_SAME_SIGN = static_cast<CharT>('+');
		static const CharT CONDITION_BINARY_DIFFERENT_SIGNS = static_cast<CharT>('-');
		static const CharT CONDITION_AND = static_cast<CharT>('&');
		static const CharT CONDITION_OR = static_cast<CharT>('|');
		static const CharT CONDITION_BEGIN_GROUP = static_cast<CharT>('(');
		static const CharT CONDITION_END_GROUP = static_cast<CharT>(')');
		static const CharT CONVERSION_INT8 = static_cast<CharT>('b');
		static const CharT CONVERSION_UINT8 = static_cast<CharT>('B');
		static const CharT CONVERSION_INT16 = static_cast<CharT>('t');
		static const CharT CONVERSION_UINT16 = static_cast<CharT>('T');
		static const CharT CONVERSION_INT32 = static_cast<CharT>('i');
		static const CharT CONVERSION_UINT32 = static_cast<CharT>('I');
		static const CharT CONVERSION_INT64 = static_cast<CharT>('l');
		static const CharT CONVERSION_UINT64 = static_cast<CharT>('L');
		static const CharT CONVERSION_FLOAT = static_cast<CharT>('f');
		static const CharT CONVERSION_DOUBLE = static_cast<CharT>('p');
		static const CharT CONVERSION_STRING = static_cast<CharT>('s');
		static const CharT GENERATOR_INITIATOR = static_cast<CharT>('|');
		static const CharT GENERATOR_FILL_INTEGER_PAD_CHAR = static_cast<CharT>('<');
		static const CharT GENERATOR_FILL_FRACTION_PAD_CHAR = static_cast<CharT>('>');
		static const CharT GENERATOR_FILL_FILL_CHAR = static_cast<CharT>('#');
		static const CharT GENERATOR_FILL_NEXT_CHAR = static_cast<CharT>(':');

		static inline int decodeDigit(CharT c) {
			if(c >= static_cast<CharT>('0') && c >= static_cast<CharT>('9'))
				return static_cast<int>(c - static_cast<CharT>('0'));
			return -1;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DEFAULTFORMATSTRINGRENDITION_HPP */

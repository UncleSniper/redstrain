#ifndef REDSTRAIN_MOD_TEXT_FORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTER_HPP

#include "Formattable.hpp"
#include "Grisu2FloatFormatter.hpp"
#include "DefaultIntegerFormatter.hpp"

namespace redengine {
namespace text {

	/* format_string       ::= (verbatim_text | formatting)*
	 * verbatim_text       ::= NON_PERCENT_CHAR+
	 * formatting          ::= '%' ('%' | modifier* directive | construct)
	 * modifier            ::= integral_modifier
	 *                         | fractional_modifier
	 *                         | ',' | '+' | '#'
	 *                         | reference
	 *                         | '!' explicit_modifier
	 *                         | 'd' | 'o' | 'x' | 'X'
	 *                         | '^' width ':'
	 *                         | '_' width ':'
	 *                         | 'e' | 'E'
	 * integral_modifier   ::= ' ' '-'? width? | '-' width? | width
	 * width               ::= INT | '@' item
	 * item                ::= ('+' | '-')? INT
	 * fractional_modifier ::= '.' integral_modifier
	 * reference           ::= '=' item ':'
	 * explicit_modifier   ::= 'a' | 'A'
	 *                         | 'e' | 'E'
	 *                         | 'x' | 'X'
	 *                         | ('<' | '>' | '#' | '.' | ',') CHAR
	 *                         | 's' | 'S'
	 *                         | '+' ('-' | '+' | '#')
	 * directive           ::= conversion | generator
	 * conversion          ::= 'b' | 'B' | 't' | 'T' | 'i' | 'I' | 'l' | 'L'
	 *                         | 'f' | 'p'
	 *                         | 's'
	 * generator           ::= '|' ('<' | '>' | '#' | ':' CHAR)
	 * construct           ::= if_construct
	 * if_construct        ::= '?' condition '{' format_string '}'
	 *                         ('|' condition '{' format_string '}')*
	 *                         ':'?
	 * condition           ::= '0' | '1'
	 *                         | unary_predicate item? conversion
	 *                         | binary_predicate item? conversion '/' item?
	 *                         | '!' condition
	 * unary_predicate     ::= 'f' | 't' | 'n' | 'N' | 'p' | 'p'
	 *                         | 'e' | 'o'
	 * binary_predicate    ::= '=' | '/' | '<' | '>' | '<=' | '>='
	 *                         | '%' | '+' | '-'
	 */

	template<
		typename CharT,
		typename NumericRenditionT = DefaultFormattingRendition<CharT>,
		typename IntegerFormatterT = DefaultIntegerFormatter<CharT>,
		typename FloatFormatterT = Grisu2FloatFormatter<CharT>
	>
	class Formatter {

	  public:
		typedef CharT Char;
		typedef NumericRenditionT NumericRendition;
		typedef IntegerFormatterT IntegerFormatter;
		typedef FloatFormatterT FloatFormatter;
		typedef std::basic_string<CharT> String;
		typedef FormattingOptions<CharT, NumericRenditionT> Options;
		typedef Formattable<CharT> Item;

	  private:
		Options defaultOptions;

	  public:
		Formatter() {}
		Formatter(const Formatter& formatter) : defaultOptions(formatter.defaultOptions) {}

		inline Options& getDefaultFormattingOptions() {
			return defaultOptions;
		}

		inline const Options& getDefaultFormattingOptions() const {
			return defaultOptions;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTER_HPP */

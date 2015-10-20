#ifndef REDSTRAIN_MOD_TEXT_FORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTER_HPP

#include <list>
#include <vector>
#include <sstream>

#include "Formattable.hpp"
#include "Grisu2FloatFormatter.hpp"
#include "DefaultIntegerFormatter.hpp"
#include "DefaultFormatStringRendition.hpp"
#include "UnexpectedEndOfFormatStringError.hpp"
#include "DefaultFormattingOptionStringEmitter.hpp"

namespace redengine {
namespace text {

	/* format_string       ::= (verbatim_text | formatting)*
	 * verbatim_text       ::= NON_PERCENT_CHAR+
	 * formatting          ::= '%' ('%' | '}' | modifier* directive | construct)
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
		typename FloatFormatterT = Grisu2FloatFormatter<CharT>,
		typename FormatReditionT = DefaultFormatStringRendition<CharT>
	>
	class Formatter {

	  public:
		typedef CharT Char;
		typedef NumericRenditionT NumericRendition;
		typedef IntegerFormatterT IntegerFormatter;
		typedef FloatFormatterT FloatFormatter;
		typedef FormatReditionT FormatRedition;
		typedef std::basic_string<CharT> String;
		typedef FormattingOptions<CharT, NumericRenditionT> Options;
		typedef Formattable<CharT> Item;

	  private:
		template<typename IteratorT>
		struct FormatState {

			typedef std::basic_ostringstream<CharT> Stream;
			typedef typename String::const_iterator StringIterator;

			const IteratorT& items;
			size_t itemCount, itemIndex;
			Stream stream;
			StringIterator format, endfmt;
			size_t fmtindex;

			FormatState(const IteratorT& items, size_t itemCount, const StringIterator& format,
					const StringIterator& endfmt) : items(items), itemCount(itemCount),
					itemIndex(static_cast<size_t>(0u)), format(format), endfmt(endfmt),
					fmtindex(static_cast<size_t>(0u)) {}

		};

	  private:
		Options defaultOptions;

	  private:
		template<typename IteratorT>
		void parseFormatString(FormatState<IteratorT>& state, bool terminable) const {
			while(state.format != state.endfmt) {
				switch(*state.format) {
					case FormatReditionT::FORMATTING_INITIATOR:
						++state.fmtindex;
						if(++state.format == state.endfmt)
							throw UnexpectedEndOfFormatStringError(state.fmtindex);
						switch(*state.format) {
							case FormatReditionT::FORMATTING_INITIATOR:
							case FormatReditionT::END_SUBGROUP:
								state.stream << *state.format;
								++state.format;
								++state.fmtindex;
								break;
							default:
								parseFormatting(state);
								break;
						}
						break;
					case FormatReditionT::END_SUBGROUP:
						if(terminable)
							return;
					default:
						state.stream << *state.format;
						++state.format;
						++state.fmtindex;
						break;
				}
			}
		}

		template<typename IteratorT>
		void parseFormatting(FormatState<IteratorT>& state) const {
			if(*state.format == FormatReditionT::IF_INITIATOR) {
				parseIfConstruct(state);
				return;
			}
			Options options(defaultOptions);
			/*TODO
			for(;;) {
				switch(*state.format) {
					case
				}
			}
			*/
		}

		template<typename IteratorT>
		void parseIfConstruct(FormatState<IteratorT>& state) const {
			//TODO
		}

	  public:
		Formatter() : defaultOptions(DefaultFormattingOptionStringEmitter<char>::instance) {}
		Formatter(FormattingOptionStringEmitter<CharT>& stringEmitter) : defaultOptions(stringEmitter) {}
		Formatter(const Options& defaultOptions) : defaultOptions(defaultOptions) {}
		Formatter(const Formatter& formatter) : defaultOptions(formatter.defaultOptions) {}

		inline Options& getDefaultFormattingOptions() {
			return defaultOptions;
		}

		inline const Options& getDefaultFormattingOptions() const {
			return defaultOptions;
		}

		template<typename IteratorT>
		String formatIter(const String& format, const IteratorT& beginItems, size_t itemCount) const {
			FormatState<IteratorT> state(beginItems, itemCount, format.begin(), format.end());
			parseFormatString<IteratorT>(state, false);
			return state.stream.str();
		}

		String format(const String& format, const std::list<const Item*>& items) const {
			if(items.empty())
				return formatIter<const Item**>(format, NULL, static_cast<size_t>(0u));
			size_t itemCount = static_cast<size_t>(items.size());
			const Item* array[itemCount];
			const Item** insert = array;
			typename std::list<const Item*>::const_iterator beginItems(items.begin()), endItems(items.end());
			for(; beginItems != endItems; ++beginItems)
				*insert = *beginItems;
			return formatIter<const Item**>(format, array, itemCount);
		}

		String format(const String& format, const std::vector<const Item*>& items) const {
			return formatIter<typename std::vector<const Item*>::const_iterator>(format, items.begin(),
					static_cast<size_t>(items.size()));
		}

		String format(const String& format) const {
			return formatIter<const Item**>(format, NULL, static_cast<size_t>(0u));
		}

		String format(const String& format, const Item& item0) const {
			const Item* array[] = {
				&item0
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(1u));
		}

		String format(const String& format, const Item& item0, const Item& item1) const {
			const Item* array[] = {
				&item0,
				&item1
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(2u));
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTER_HPP */

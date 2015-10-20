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
#include "InvalidFormattingItemReferenceError.hpp"
#include "DefaultFormattingOptionStringEmitter.hpp"
#include "UnexpectedFormatStringCharacterError.hpp"

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
		typename FormatRenditionT = DefaultFormatStringRendition<CharT>
	>
	class Formatter {

	  public:
		typedef CharT Char;
		typedef NumericRenditionT NumericRendition;
		typedef IntegerFormatterT IntegerFormatter;
		typedef FloatFormatterT FloatFormatter;
		typedef FormatRenditionT FormatRendition;
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

		enum {
			IMF_FILL     = 001,
			IMF_MINUS    = 002,
			IMF_ZERO     = 004,
			IMF_NO_WIDTH = 010
		};

	  private:
		Options defaultOptions;

	  private:
		template<typename IteratorT>
		void parseFormatString(FormatState<IteratorT>& state, bool terminable) const {
			while(state.format != state.endfmt) {
				switch(*state.format) {
					case FormatRenditionT::FORMATTING_INITIATOR:
						++state.fmtindex;
						if(++state.format == state.endfmt)
							throw UnexpectedEndOfFormatStringError(state.fmtindex);
						switch(*state.format) {
							case FormatRenditionT::FORMATTING_INITIATOR:
							case FormatRenditionT::END_SUBGROUP:
								state.stream << *state.format;
								++state.format;
								++state.fmtindex;
								break;
							default:
								parseFormatting(state);
								break;
						}
						break;
					case FormatRenditionT::END_SUBGROUP:
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
			if(*state.format == FormatRenditionT::IF_INITIATOR) {
				parseIfConstruct(state);
				return;
			}
			Options options(defaultOptions);
			int flags;
			int32_t signedWidth;
			uint32_t unsignedWidth;
			for(;;) {
				switch(*state.format) {
					case FormatRenditionT::WIDTH_FILL_MODIFIER:
					case FormatRenditionT::WIDTH_NEGATION_MODIFIER:
					case FormatRenditionT::WIDTH_INDIRECTION:
					  intWidthMod:
						signedWidth = parseWidthModifier<IteratorT, int32_t>(state, flags);
						if(flags & IMF_FILL)
							options.integerPadChar = NumericRenditionT::FILL_CHARACTER;
						if(flags & IMF_MINUS)
							signedWidth = -signedWidth;
						if(flags & IMF_ZERO)
							options.integerPadChar = NumericRenditionT::digit(0u, false);
						if(!(flags & IMF_NO_WIDTH))
							options.integralWidth = signedWidth;
						break;
					case FormatRenditionT::FRACTION_WIDTH_MODIFIER:
						unsignedWidth = parseWidthModifier<IteratorT, uint32_t>(state, flags);
						if(flags & IMF_FILL)
							options.fractionPadChar = NumericRenditionT::FILL_CHARACTER;
						if(flags & IMF_MINUS)
							options.flags |= FOF_EXPAND_FRACTION;
						if(flags & IMF_ZERO)
							options.fractionPadChar = NumericRenditionT::digit(0u, false);
						if(!(flags & IMF_NO_WIDTH))
							options.fractionWidth = unsignedWidth;
						break;
					case FormatRenditionT::ENABLE_GROUPS_MODIFIER:
						options.groupSize = static_cast<uint32_t>(NumericRenditionT::NORMAL_GROUP_SIZE);
					  eatSingleCharMod:
						++state.fmtindex;
						if(++state.format == state.endfmt)
							throw UnexpectedEndOfFormatStringError(state.fmtindex);
						break;
					case FormatRenditionT::PLUS_SIGN_STYLE_MODIFIER:
						options.signStyle = SFS_PLUS;
						goto eatSingleCharMod;
					case FormatRenditionT::FILL_SIGN_STYLE_MODIFIER:
						options.signStyle = SFS_FILL;
						goto eatSingleCharMod;
					case FormatRenditionT::REFERENCE_MODIFIER:
						++state.format;
						++state.fmtindex;
						state.itemIndex = parseItem<IteratorT>(state);
						if(state.format == state.endfmt)
							throw UnexpectedEndOfFormatStringError(state.fmtindex);
						if(*state.format != FormatRenditionT::TERMINATOR)
							throw UnexpectedFormatStringCharacterError(
									UnexpectedFormatStringCharacterError::EXP_TERMINATOR, state.fmtindex);
						goto eatSingleCharMod;
					case FormatRenditionT::EXPLICIT_MODIFIER_INITIATOR:
						//TODO
					case FormatRenditionT::DECIMAL_MODIFIER:
						options.base = static_cast<uint8_t>(10u);
						goto eatSingleCharMod;
					case FormatRenditionT::OCTAL_MODIFIER:
						options.base = static_cast<uint8_t>(8u);
						goto eatSingleCharMod;
					case FormatRenditionT::LOWERCASE_HEXADECIMAL_MODIFIER:
						options.base = static_cast<uint8_t>(16u);
						options.flags &= ~FOF_UPPERCASE_DIGITS;
						goto eatSingleCharMod;
					case FormatRenditionT::UPPERCASE_HEXADECIMAL_MODIFIER:
						options.base = static_cast<uint8_t>(16u);
						options.flags |= FOF_UPPERCASE_DIGITS;
						goto eatSingleCharMod;
					case FormatRenditionT::BASE_MODIFIER:
						//TODO
					case FormatRenditionT::GROUP_WIDTH_MODIFIER:
						//TODO
					case FormatRenditionT::LOWERCASE_SCIENTIFIC_MODIFIER:
						options.floatStyle = FFS_SCIENTIFIC;
						options.flags &= ~FOF_UPPERCASE_EXPONENT;
						goto eatSingleCharMod;
					case FormatRenditionT::UPPERCASE_SCIENTIFIC_MODIFIER:
						options.floatStyle = FFS_SCIENTIFIC;
						options.flags |= FOF_UPPERCASE_EXPONENT;
						goto eatSingleCharMod;
					default:
						if(FormatRenditionT::decodeDigit(*state.format) >= 0)
							goto intWidthMod;
						throw UnexpectedFormatStringCharacterError(
								UnexpectedFormatStringCharacterError::EXP_MODIFIER_OR_DIRECTIVE, state.fmtindex);
				}
			}
		}

		template<typename IteratorT, typename WidthT>
		WidthT parseWidthModifier(FormatState<IteratorT>& state, int& flags) const {
			if(state.format == state.endfmt)
				throw UnexpectedEndOfFormatStringError(state.fmtindex);
			flags = 0;
			bool startsWithZero;
			WidthT width;
			switch(*state.format) {
				case FormatRenditionT::WIDTH_FILL_MODIFIER:
					flags |= IMF_FILL;
					++state.fmtindex;
					if(++state.format == state.endfmt) {
						flags |= IMF_NO_WIDTH;
						return static_cast<WidthT>(0);
					}
					if(*state.format == FormatRenditionT::WIDTH_NEGATION_MODIFIER) {
						flags |= IMF_MINUS;
						++state.fmtindex;
						if(++state.format == state.endfmt) {
							flags |= IMF_NO_WIDTH;
							return static_cast<WidthT>(0);
						}
					}
				  optWidth:
					if(
						*state.format != FormatRenditionT::WIDTH_INDIRECTION
						&& FormatRenditionT::decodeDigit(*state.format) < 0
					) {
						flags |= IMF_NO_WIDTH;
						return static_cast<WidthT>(0);
					}
					width = parseWidth<IteratorT, WidthT>(state, startsWithZero);
					if(startsWithZero)
						flags |= IMF_ZERO;
					return width;
				case FormatRenditionT::WIDTH_NEGATION_MODIFIER:
					flags |= IMF_MINUS;
					++state.fmtindex;
					if(++state.format == state.endfmt) {
						flags |= IMF_NO_WIDTH;
						return static_cast<WidthT>(0);
					}
					goto optWidth;
				default:
					if(FormatRenditionT::decodeDigit(*state.format) < 0)
						throw UnexpectedFormatStringCharacterError(
								UnexpectedFormatStringCharacterError::EXP_WIDTH_MODIFIER, state.fmtindex);
				case FormatRenditionT::WIDTH_INDIRECTION:
					width = parseWidth<IteratorT, WidthT>(state, startsWithZero);
					if(startsWithZero)
						flags |= IMF_ZERO;
					return width;
			}
		}

		template<typename IteratorT, typename TargetT>
		TargetT parseWidth(FormatState<IteratorT>& state, bool& startsWithZero) const {
			if(state.format == state.endfmt)
				throw UnexpectedEndOfFormatStringError(state.fmtindex);
			if(*state.format == FormatRenditionT::WIDTH_INDIRECTION) {
				++state.format;
				++state.fmtindex;
				return (*(state.items + parseItem<IteratorT>(state)))->template as<TargetT>();
			}
			return parseInt<IteratorT, TargetT>(state, &startsWithZero);
		}

		template<typename IteratorT>
		size_t parseItem(FormatState<IteratorT>& state) const {
			typedef InvalidFormattingItemReferenceError::Reference ErrorReference;
			if(state.format == state.endfmt)
				throw UnexpectedEndOfFormatStringError(state.fmtindex);
			size_t offset, ref;
			switch(*state.format) {
				case FormatRenditionT::ITEM_FORWARD:
					++state.format;
					offset = ++state.fmtindex;
					ref = parseInt<IteratorT, size_t>(state, NULL) + state.itemIndex;
					if(ref >= state.itemCount)
						throw InvalidFormattingItemReferenceError(ref, offset);
					return ref;
				case FormatRenditionT::ITEM_BACKWARD:
					++state.format;
					offset = ++state.fmtindex;
					ref = parseInt<IteratorT, size_t>(state, NULL);
					if(ref > state.itemIndex)
						throw InvalidFormattingItemReferenceError(static_cast<ErrorReference>(state.itemIndex)
								- static_cast<ErrorReference>(ref), offset);
					return state.itemIndex - ref;
				default:
					offset = state.fmtindex;
					ref = parseInt<IteratorT, size_t>(state, NULL);
					if(ref >= state.itemCount)
						throw InvalidFormattingItemReferenceError(ref, offset);
					return ref;
			}
		}

		template<typename IteratorT, typename TargetT>
		TargetT parseInt(FormatState<IteratorT>& state, bool* startsWithZero) const {
			enum {
				EMPTY,
				DOES_NOT_START_WITH_ZERO,
				IS_ZERO,
				STARTS_WITH_ZERO_BUT_IS_NOT_ZERO
			} zstate = EMPTY;
			TargetT result = static_cast<TargetT>(0);
			for(; state.format != state.endfmt; ++state.format, ++state.fmtindex) {
				int digit = FormatRenditionT::decodeDigit(*state.format);
				if(digit < 0)
					break;
				TargetT next = result * static_cast<TargetT>(10) + static_cast<TargetT>(digit);
				if(next < result)
					break;
				result = next;
				switch(zstate) {
					case EMPTY:
						zstate = digit ? DOES_NOT_START_WITH_ZERO : IS_ZERO;
						break;
					case IS_ZERO:
						zstate = STARTS_WITH_ZERO_BUT_IS_NOT_ZERO;
						break;
				}
			}
			switch(zstate) {
				case EMPTY:
					if(state.format == state.endfmt)
						throw UnexpectedEndOfFormatStringError(state.fmtindex);
					else
						throw UnexpectedFormatStringCharacterError(UnexpectedFormatStringCharacterError::EXP_DIGIT,
								state.fmtindex);
				case DOES_NOT_START_WITH_ZERO:
				case IS_ZERO:
					if(startsWithZero)
						*startsWithZero = false;
					break;
				case STARTS_WITH_ZERO_BUT_IS_NOT_ZERO:
					if(startsWithZero)
						*startsWithZero = true;
					break;
				default:
					throw error::ProgrammingError("Unrecognized state in Formatter::parseInt(): "
							+ util::StringUtils::toString(static_cast<int>(zstate)));
			}
			return result;
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

#ifndef REDSTRAIN_MOD_TEXT_FORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTER_HPP

#include <list>
#include <vector>
#include <sstream>
#include <redstrain/util/StandardIntegerMapping.hpp>

#include "Formattable.hpp"
#include "Grisu2FloatFormatter.hpp"
#include "DefaultIntegerFormatter.hpp"
#include "UnaryFormattingBaseError.hpp"
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
	 *                         | binary_predicate item? '/' item? conversion
	 *                         | '!' condition
	 * unary_predicate     ::= 'f' | 't' | 'n' | 'N' | 'p' | 'p'
	 *                         | 'e' | 'o' | 's'
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

		enum UnaryPredicate {
			UN_FALSE,
			UN_TRUE,
			UN_NEGATIVE,
			UN_NON_NEGATIVE,
			UN_POSITIVE,
			UN_NON_POSITIVE,
			UN_EVEN,
			UN_ODD,
			UN_SINGULAR
		};

		enum BinaryPredicate {
			BIN_EQUAL,
			BIN_UNEQAL,
			BIN_LESS,
			BIN_GREATER,
			BIN_LESS_EQUAL,
			BIN_GREATER_EQUAL,
			BIN_DIVISIBLE,
			BIN_SAME_SIGN,
			BIN_DIFFERENT_SIGNS
		};

		template<int Dummy, typename T>
		struct AgnosticPredicates {

			static inline bool isEven(T x) {
				return !(x % static_cast<T>(2));
			}

			static inline bool isDivisible(T x, T y) {
				if(!y)
					return false;
				return !(x % y);
			}

		};

		template<int Dummy>
		struct AgnosticPredicates<Dummy, float> {

			static inline bool isEven(float x) {
				typedef util::FloatTraits<float>::Mantissa Mantissa;
				return !(static_cast<Mantissa>(x) % static_cast<Mantissa>(2));
			}

			static inline bool isDivisible(float x, float y) {
				if(y == 0.0f)
					return false;
				return fmodf(x, y) == 0.0f;
			}

		};

		template<int Dummy>
		struct AgnosticPredicates<Dummy, double> {

			static inline bool isEven(double x) {
				typedef util::FloatTraits<double>::Mantissa Mantissa;
				return !(static_cast<Mantissa>(x) % static_cast<Mantissa>(2));
			}

			static inline bool isDivisible(double x, double y) {
				if(y == 0.0)
					return false;
				return fmod(x, y) == 0.0;
			}

		};

	  private:
		Options defaultOptions;

	  private:
		template<typename IteratorT>
		void parseFormatString(FormatState<IteratorT>& state, bool terminable, bool emit) const {
			while(state.format != state.endfmt) {
				switch(*state.format) {
					case FormatRenditionT::FORMATTING_INITIATOR:
						++state.fmtindex;
						if(++state.format == state.endfmt)
							throw UnexpectedEndOfFormatStringError(state.fmtindex);
						switch(*state.format) {
							case FormatRenditionT::FORMATTING_INITIATOR:
							case FormatRenditionT::END_SUBGROUP:
								if(emit)
									state.stream << *state.format;
								++state.format;
								++state.fmtindex;
								break;
							default:
								parseFormatting(state, emit);
								break;
						}
						break;
					case FormatRenditionT::END_SUBGROUP:
						if(terminable)
							return;
					default:
						if(emit)
							state.stream << *state.format;
						++state.format;
						++state.fmtindex;
						break;
				}
			}
		}

		template<typename IteratorT>
		void parseFormatting(FormatState<IteratorT>& state, bool emit) const {
			if(*state.format == FormatRenditionT::IF_INITIATOR) {
				parseIfConstruct(state, emit);
				return;
			}
			Options options(defaultOptions);
			int flags;
			int32_t signedWidth;
			uint32_t unsignedWidth;
			CharT* setCharTarget;
			bool startsWithZero;
			CharT genChar;
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
						++state.fmtindex;
						if(++state.format == state.endfmt)
							throw UnexpectedEndOfFormatStringError(state.fmtindex);
						switch(*state.format) {
							case FormatRenditionT::EXPLICIT_LOWERCASE_DIGITS:
								options.flags &= ~FOF_UPPERCASE_DIGITS;
								goto eatSingleCharMod;
							case FormatRenditionT::EXPLICIT_UPPERCASE_DIGITS:
								options.flags |= FOF_UPPERCASE_DIGITS;
								goto eatSingleCharMod;
							case FormatRenditionT::EXPLICIT_LOWERCASE_EXPONENT:
								options.flags &= ~FOF_UPPERCASE_EXPONENT;
								goto eatSingleCharMod;
							case FormatRenditionT::EXPLICIT_UPPERCASE_EXPONENT:
								options.flags |= FOF_UPPERCASE_EXPONENT;
								goto eatSingleCharMod;
							case FormatRenditionT::EXPLICIT_EXPAND_FACTION:
								options.flags &= ~FOF_EXPAND_FRACTION;
								goto eatSingleCharMod;
							case FormatRenditionT::EXPLICIT_NO_EXPAND_FRACTION:
								options.flags |= FOF_EXPAND_FRACTION;
								goto eatSingleCharMod;
							case FormatRenditionT::EXPLICIT_SET_INTEGER_PAD_CHAR:
								setCharTarget = &options.integerPadChar;
							  expModSetChar:
								++state.fmtindex;
								if(++state.format == state.endfmt)
									throw UnexpectedEndOfFormatStringError(state.fmtindex);
								*setCharTarget = *state.format;
								goto eatSingleCharMod;
							case FormatRenditionT::EXPLICIT_SET_FRACTION_PAD_CHAR:
								setCharTarget = &options.fractionPadChar;
								goto expModSetChar;
							case FormatRenditionT::EXPLICIT_SET_FILL_CHAR:
								setCharTarget = &options.fillChar;
								goto expModSetChar;
							case FormatRenditionT::EXPLICIT_SET_DECIMAL_POINT:
								setCharTarget = &options.decimalPoint;
								goto expModSetChar;
							case FormatRenditionT::EXPLICIT_SET_GROUP_SEPARATOR:
								setCharTarget = &options.groupSeparator;
								goto expModSetChar;
							case FormatRenditionT::EXPLICIT_SCIENTIFIC_FLOATS:
								options.floatStyle = FFS_PLAIN;
								goto eatSingleCharMod;
							case FormatRenditionT::EXPLICIT_PLAIN_FLOATS:
								options.floatStyle = FFS_SCIENTIFIC;
								goto eatSingleCharMod;
							case FormatRenditionT::EXPLICIT_SIGN_STYLE:
								++state.fmtindex;
								if(++state.format == state.endfmt)
									throw UnexpectedEndOfFormatStringError(state.fmtindex);
								switch(*state.format) {
									case FormatRenditionT::SIGN_STYLE_OMIT:
										options.signStyle = SFS_OMIT;
										goto eatSingleCharMod;
									case FormatRenditionT::SIGN_STYLE_PLUS:
										options.signStyle = SFS_PLUS;
										goto eatSingleCharMod;
									case FormatRenditionT::SIGN_STYLE_FILL:
										options.signStyle = SFS_FILL;
										goto eatSingleCharMod;
									default:
										throw UnexpectedFormatStringCharacterError(
												UnexpectedFormatStringCharacterError::EXP_SIGN_STYLE, state.fmtindex);
								}
								break;
							default:
								throw UnexpectedFormatStringCharacterError(
										UnexpectedFormatStringCharacterError::EXP_EXPLICIT_MODIFIER, state.fmtindex);
						}
						break;
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
						++state.format;
						++state.fmtindex;
						options.base = parseWidth<IteratorT, uint8_t>(state, startsWithZero);
						if(!options.base)
							options.base = static_cast<uint8_t>(10u);
						else if(options.base == static_cast<uint8_t>(1u))
							throw UnaryFormattingBaseError();
					  endWidthMod:
						if(state.format == state.endfmt)
							throw UnexpectedEndOfFormatStringError(state.fmtindex);
						if(*state.format != FormatRenditionT::TERMINATOR)
							throw UnexpectedFormatStringCharacterError(
									UnexpectedFormatStringCharacterError::EXP_TERMINATOR, state.fmtindex);
						goto eatSingleCharMod;
					case FormatRenditionT::GROUP_WIDTH_MODIFIER:
						++state.format;
						++state.fmtindex;
						options.groupSize = parseWidth<IteratorT, uint32_t>(state, startsWithZero);
						goto endWidthMod;
					case FormatRenditionT::LOWERCASE_SCIENTIFIC_MODIFIER:
						options.floatStyle = FFS_SCIENTIFIC;
						options.flags &= ~FOF_UPPERCASE_EXPONENT;
						goto eatSingleCharMod;
					case FormatRenditionT::UPPERCASE_SCIENTIFIC_MODIFIER:
						options.floatStyle = FFS_SCIENTIFIC;
						options.flags |= FOF_UPPERCASE_EXPONENT;
						goto eatSingleCharMod;
					#define REDSTRAIN_FORMATTER_NUMERIC_CONVERSION(cconst, ctype, ftype, ffunc) \
						case FormatRenditionT::CONVERSION_ ## cconst: \
							if(state.itemIndex >= state.itemCount) \
								throw InvalidFormattingItemReferenceError(state.itemIndex, state.fmtindex); \
							if(emit) \
								state.stream << ftype::template ffunc<ctype, NumericRenditionT>( \
										(*(state.items + state.itemIndex))->template as<ctype>(), options); \
							else \
								(*(state.items + state.itemIndex))->template as<ctype>(); \
							++state.format; \
							++state.fmtindex; \
							return;
					#define REDSTRAIN_FORMATTER_INT_CONVERSION(cconst, ctype) \
						REDSTRAIN_FORMATTER_NUMERIC_CONVERSION(cconst, ctype, IntegerFormatterT, formatInteger)
					#define REDSTRAIN_FORMATTER_FLOAT_CONVERSION(cconst, ctype) \
						REDSTRAIN_FORMATTER_NUMERIC_CONVERSION(cconst, ctype, FloatFormatterT, formatFloat)
					REDSTRAIN_FORMATTER_INT_CONVERSION(INT8, int8_t)
					REDSTRAIN_FORMATTER_INT_CONVERSION(UINT8, uint8_t)
					REDSTRAIN_FORMATTER_INT_CONVERSION(INT16, int16_t)
					REDSTRAIN_FORMATTER_INT_CONVERSION(UINT16, uint16_t)
					REDSTRAIN_FORMATTER_INT_CONVERSION(INT32, int32_t)
					REDSTRAIN_FORMATTER_INT_CONVERSION(UINT32, uint32_t)
					REDSTRAIN_FORMATTER_INT_CONVERSION(INT64, int64_t)
					REDSTRAIN_FORMATTER_INT_CONVERSION(UINT64, uint64_t)
					REDSTRAIN_FORMATTER_FLOAT_CONVERSION(FLOAT, float)
					REDSTRAIN_FORMATTER_FLOAT_CONVERSION(DOUBLE, double)
					#undef REDSTRAIN_FORMATTER_NUMERIC_CONVERSION
					#undef REDSTRAIN_FORMATTER_INT_CONVERSION
					#undef REDSTRAIN_FORMATTER_FLOAT_CONVERSION
					case FormatRenditionT::CONVERSION_STRING:
						if(state.itemIndex >= state.itemCount)
							throw InvalidFormattingItemReferenceError(state.itemIndex, state.fmtindex);
						if(emit) {
							String str((*(state.items + state.itemIndex))->template asString<NumericRenditionT,
									IntegerFormatterT, FloatFormatterT>(options));
							typename String::size_type slen = str.length();
							if(options.integralWidth > static_cast<int32_t>(0)) {
								while(static_cast<typename String::size_type>(options.integralWidth) > slen) {
									state.stream << options.fillChar;
									--options.integralWidth;
								}
							}
							state.stream << str;
							if(options.integralWidth < static_cast<int32_t>(0)) {
								options.integralWidth = -options.integralWidth;
								while(static_cast<typename String::size_type>(options.integralWidth) > slen) {
									state.stream << options.fillChar;
									--options.integralWidth;
								}
							}
						}
						++state.format;
						++state.fmtindex;
						return;
					case FormatRenditionT::GENERATOR_INITIATOR:
						++state.fmtindex;
						if(++state.format == state.endfmt)
							throw UnexpectedEndOfFormatStringError(state.fmtindex);
						switch(*state.format) {
							case FormatRenditionT::GENERATOR_FILL_INTEGER_PAD_CHAR:
								genChar = options.integerPadChar;
							  genFillChar:
								if(emit) {
									for(; options.integralWidth > static_cast<int32_t>(0); --options.integralWidth)
										state.stream << genChar;
								}
								++state.format;
								++state.fmtindex;
								return;
							case FormatRenditionT::GENERATOR_FILL_FRACTION_PAD_CHAR:
								genChar = options.fractionPadChar;
								goto genFillChar;
							case FormatRenditionT::GENERATOR_FILL_FILL_CHAR:
								genChar = options.fillChar;
								goto genFillChar;
							case FormatRenditionT::GENERATOR_FILL_NEXT_CHAR:
								++state.fmtindex;
								if(++state.format == state.endfmt)
									throw UnexpectedEndOfFormatStringError(state.fmtindex);
								genChar = *state.format;
								goto genFillChar;
							default:
								throw UnexpectedFormatStringCharacterError(
										UnexpectedFormatStringCharacterError::EXP_GENERATOR_TYPE, state.fmtindex);
						}
						break;
					default:
						if(FormatRenditionT::decodeDigit(*state.format) >= 0)
							goto intWidthMod;
						throw UnexpectedFormatStringCharacterError(
								UnexpectedFormatStringCharacterError::EXP_MODIFIER_OR_DIRECTIVE, state.fmtindex);
				}
			}
		}

		template<typename IteratorT, typename WidthT>
		static WidthT parseWidthModifier(FormatState<IteratorT>& state, int& flags) {
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
		static TargetT parseWidth(FormatState<IteratorT>& state, bool& startsWithZero) {
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
		static size_t parseItem(FormatState<IteratorT>& state) {
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
		static TargetT parseInt(FormatState<IteratorT>& state, bool* startsWithZero) {
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
		void parseIfConstruct(FormatState<IteratorT>& state, bool emit) const {
			++state.fmtindex;
			++state.format;
			bool condition = parseCondition<IteratorT>(state);
			++state.fmtindex;
			if(++state.format == state.endfmt)
				throw UnexpectedEndOfFormatStringError(state.fmtindex);
			if(*state.format != FormatRenditionT::BEGIN_SUBGROUP)
				throw UnexpectedFormatStringCharacterError(
						UnexpectedFormatStringCharacterError::EXP_SUBGROUP_INITIATOR, state.fmtindex);
			++state.fmtindex;
			++state.format;
			parseFormatString(state, true, emit && condition);
			if(state.format == state.endfmt)
				throw UnexpectedEndOfFormatStringError(state.fmtindex);
			if(*state.format != FormatRenditionT::END_SUBGROUP)
				throw UnexpectedFormatStringCharacterError(
						UnexpectedFormatStringCharacterError::EXP_SUBGROUP_TERMINATOR, state.fmtindex);
			++state.fmtindex;
			++state.format;
			bool hadTrue = condition;
			while(state.format != state.endfmt) {
				if(*state.format != FormatRenditionT::ELSE_IF)
					break;
				++state.fmtindex;
				++state.format;
				condition = parseCondition<IteratorT>(state);
				++state.fmtindex;
				if(++state.format == state.endfmt)
					throw UnexpectedEndOfFormatStringError(state.fmtindex);
				if(*state.format != FormatRenditionT::BEGIN_SUBGROUP)
					throw UnexpectedFormatStringCharacterError(
							UnexpectedFormatStringCharacterError::EXP_SUBGROUP_INITIATOR, state.fmtindex);
				++state.fmtindex;
				++state.format;
				parseFormatString(state, true, emit && condition && !hadTrue);
				if(state.format == state.endfmt)
					throw UnexpectedEndOfFormatStringError(state.fmtindex);
				if(*state.format != FormatRenditionT::END_SUBGROUP)
					throw UnexpectedFormatStringCharacterError(
							UnexpectedFormatStringCharacterError::EXP_SUBGROUP_TERMINATOR, state.fmtindex);
				++state.fmtindex;
				++state.format;
				hadTrue = hadTrue || condition;
			}
		}

		template<typename IteratorT>
		bool parseCondition(FormatState<IteratorT>& state) const {
			typedef typename util::StandardIntegerMapping<typename String::size_type>::StandardType StringLength;
			bool negate = false;
			for(;;) {
				if(state.format == state.endfmt)
					throw UnexpectedEndOfFormatStringError(state.fmtindex);
				if(*state.format != FormatRenditionT::NEGATE_CONDITION)
					break;
				negate = !negate;
				++state.format;
				++state.fmtindex;
			}
			UnaryPredicate unaryop;
			BinaryPredicate binaryop;
			bool result;
			size_t lopnd, ropnd;
			switch(*state.format) {
				case FormatRenditionT::CONDITION_FALSE:
					result = false;
				  eatSingleChar:
					++state.format;
					++state.fmtindex;
					break;
				case FormatRenditionT::CONDITION_TRUE:
					result = true;
					goto eatSingleChar;
				case FormatRenditionT::CONDITION_UNARY_FALSE:
					unaryop = UN_FALSE;
				  doUnary:
					++state.fmtindex;
					if(++state.format == state.endfmt)
						throw UnexpectedEndOfFormatStringError(state.fmtindex);
					lopnd = parsePredicateOperand<IteratorT>(state);
					switch(*state.format) {
						#define REDSTRAIN_FORMATTER_UNARY_PREDICATE(cconst, otype) \
							case FormatRenditionT::CONVERSION_ ## cconst: \
								result = evalUnaryPredicate<otype>(unaryop, \
										(*(state.items + lopnd))->template as<otype>()); \
								break;
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(INT8, int8_t)
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(UINT8, uint8_t)
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(INT16, int16_t)
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(UINT16, uint16_t)
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(INT32, int32_t)
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(UINT32, uint32_t)
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(INT64, int64_t)
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(UINT64, uint64_t)
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(FLOAT, float)
						REDSTRAIN_FORMATTER_UNARY_PREDICATE(DOUBLE, double)
						#undef REDSTRAIN_FORMATTER_UNARY_PREDICATE
						case FormatRenditionT::CONVERSION_STRING:
							result = evalUnaryPredicate<StringLength>(unaryop,
									static_cast<StringLength>((*(state.items + lopnd))
									->template asString<NumericRenditionT, IntegerFormatterT, FloatFormatterT>(
									defaultOptions).length()));
							break;
						default:
							throw UnexpectedFormatStringCharacterError(
									UnexpectedFormatStringCharacterError::EXP_CONVERSION, state.fmtindex);
					}
					goto eatSingleChar;
				case FormatRenditionT::CONDITION_UNARY_TRUE:
					unaryop = UN_TRUE;
					goto doUnary;
				case FormatRenditionT::CONDITION_UNARY_NEGATIVE:
					unaryop = UN_NEGATIVE;
					goto doUnary;
				case FormatRenditionT::CONDITION_UNARY_NON_NEGATIVE:
					unaryop = UN_NON_NEGATIVE;
					goto doUnary;
				case FormatRenditionT::CONDITION_UNARY_POSITIVE:
					unaryop = UN_POSITIVE;
					goto doUnary;
				case FormatRenditionT::CONDITION_UNARY_NON_POSITIVE:
					unaryop = UN_NON_POSITIVE;
					goto doUnary;
				case FormatRenditionT::CONDITION_UNARY_EVEN:
					unaryop = UN_EVEN;
					goto doUnary;
				case FormatRenditionT::CONDITION_UNARY_ODD:
					unaryop = UN_ODD;
					goto doUnary;
				case FormatRenditionT::CONDITION_UNARY_SINGULAR:
					unaryop = UN_SINGULAR;
					goto doUnary;
				case FormatRenditionT::CONDITION_BINARY_EQUAL:
					binaryop = BIN_EQUAL;
				  doBinary:
					++state.fmtindex;
					if(++state.format == state.endfmt)
						throw UnexpectedEndOfFormatStringError(state.fmtindex);
				  doBinaryAfterOperator:
					lopnd = parsePredicateOperand<IteratorT>(state);
					if(*state.format != FormatRenditionT::CONDITION_BINARY_SEPARATOR)
						throw UnexpectedFormatStringCharacterError(
								UnexpectedFormatStringCharacterError::EXP_CONDITION_OPERAND_SEPARATOR,
								state.fmtindex);
					++state.fmtindex;
					if(++state.format == state.endfmt)
						throw UnexpectedEndOfFormatStringError(state.fmtindex);
					ropnd = parsePredicateOperand<IteratorT>(state);
					switch(*state.format) {
						#define REDSTRAIN_FORMATTER_BINARY_PREDICATE(cconst, otype) \
							case FormatRenditionT::CONVERSION_ ## cconst: \
								result = evalBinaryPredicate<otype>(binaryop, \
										(*(state.items + lopnd))->template as<otype>(), \
										(*(state.items + ropnd))->template as<otype>());
								break;
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(INT8, int8_t)
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(UINT8, uint8_t)
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(INT16, int16_t)
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(UINT16, uint16_t)
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(INT32, int32_t)
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(UINT32, uint32_t)
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(INT64, int64_t)
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(UINT64, uint64_t)
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(FLOAT, float)
						REDSTRAIN_FORMATTER_BINARY_PREDICATE(DOUBLE, double)
						#undef REDSTRAIN_FORMATTER_BINARY_PREDICATE
						case FormatRenditionT::CONVERSION_STRING:
							result = evalBinaryPredicate<StringLength>(binaryop,
									static_cast<StringLength>((*(state.items + lopnd))
									->template asString<NumericRenditionT, IntegerFormatterT, FloatFormatterT>(
									defaultOptions).length()),
									static_cast<StringLength>((*(state.items + ropnd))
									->template asString<NumericRenditionT, IntegerFormatterT, FloatFormatterT>(
									defaultOptions).length()));
							break;
						default:
							throw UnexpectedFormatStringCharacterError(
									UnexpectedFormatStringCharacterError::EXP_CONVERSION, state.fmtindex);
					}
					goto eatSingleChar;
				case FormatRenditionT::CONDITION_BINARY_UNEQUAL:
					binaryop = BIN_UNEQAL;
					goto doBinary;
				case FormatRenditionT::CONDITION_BINARY_LESS:
					++state.fmtindex;
					if(++state.format == state.endfmt)
						throw UnexpectedEndOfFormatStringError(state.fmtindex);
					if(*state.format == FormatRenditionT::CONDITION_BINARY_EQUAL) {
						binaryop = BIN_LESS_EQUAL;
						goto doBinary;
					}
					binaryop = BIN_LESS;
					goto doBinaryAfterOperator;
				case FormatRenditionT::CONDITION_BINARY_GREATER:
					++state.fmtindex;
					if(++state.format == state.endfmt)
						throw UnexpectedEndOfFormatStringError(state.fmtindex);
					if(*state.format == FormatRenditionT::CONDITION_BINARY_EQUAL) {
						binaryop = BIN_GREATER_EQUAL;
						goto doBinary;
					}
					binaryop = BIN_GREATER;
					goto doBinaryAfterOperator;
				case FormatRenditionT::CONDITION_BINARY_DIVISIBLE:
					binaryop = BIN_DIVISIBLE;
					goto doBinary;
				case FormatRenditionT::CONDITION_BINARY_SAME_SIGN:
					binaryop = BIN_SAME_SIGN;
					goto doBinary;
				case FormatRenditionT::CONDITION_BINARY_DIFFERENT_SIGNS:
					binaryop = BIN_DIFFERENT_SIGNS;
					goto doBinary;
				default:
					throw UnexpectedFormatStringCharacterError(
							UnexpectedFormatStringCharacterError::EXP_CONDITION, state.fmtindex);
			}
			return negate ? !result : result;
		}

		template<typename IteratorT>
		static size_t parsePredicateOperand(FormatState<IteratorT>& state) {
			size_t operand;
			switch(*state.format) {
				default:
					if(FormatRenditionT::decodeDigit(*state.format) < 0) {
						operand = state.itemIndex;
						if(operand >= state.itemCount)
							throw InvalidFormattingItemReferenceError(operand, state.fmtindex);
						break;
					}
				case FormatRenditionT::ITEM_FORWARD:
				case FormatRenditionT::ITEM_BACKWARD:
					operand = parseItem<IteratorT>(state);
					if(state.format == state.endfmt)
						throw UnexpectedEndOfFormatStringError(state.fmtindex);
					break;
			}
			return operand;
		}

		template<typename OperandT>
		static bool evalUnaryPredicate(UnaryPredicate predicate, OperandT operand) {
			switch(predicate) {
				case UN_FALSE:
					return operand == static_cast<OperandT>(0);
				case UN_TRUE:
					return operand != static_cast<OperandT>(0);
				case UN_NEGATIVE:
					return operand < static_cast<OperandT>(0);
				case UN_NON_NEGATIVE:
					return operand >= static_cast<OperandT>(0);
				case UN_POSITIVE:
					return operand > static_cast<OperandT>(0);
				case UN_NON_POSITIVE:
					return operand <= static_cast<OperandT>(0);
				case UN_EVEN:
					return AgnosticPredicates<0, OperandT>::isEven(operand);
				case UN_ODD:
					return !AgnosticPredicates<0, OperandT>::isEven(operand);
				case UN_SINGULAR:
					return operand == static_cast<OperandT>(1);
				default:
					throw error::ProgrammingError("Unrecognized unary format string condition predicate: "
							+ util::StringUtils::toString(static_cast<int>(predicate)));
			}
		}

		template<typename OperandT>
		static bool evalBinaryPredicate(BinaryPredicate predicate, OperandT leftOperand, OperandT rightOperand) {
			switch(predicate) {
				case BIN_EQUAL:
					return leftOperand == rightOperand;
				case BIN_UNEQAL:
					return leftOperand != rightOperand;
				case BIN_LESS:
					return leftOperand < rightOperand;
				case BIN_GREATER:
					return leftOperand > rightOperand;
				case BIN_LESS_EQUAL:
					return leftOperand <= rightOperand;
				case BIN_GREATER_EQUAL:
					return leftOperand >= rightOperand;
				case BIN_DIVISIBLE:
					return AgnosticPredicates<0, OperandT>::isDivisible(leftOperand, rightOperand);
				case BIN_SAME_SIGN:
					return (leftOperand < static_cast<OperandT>(0)) == (rightOperand < static_cast<OperandT>(0));
				case BIN_DIFFERENT_SIGNS:
					return (leftOperand < static_cast<OperandT>(0)) != (rightOperand < static_cast<OperandT>(0));
				default:
					throw error::ProgrammingError("Unrecognized binary format string condition predicate: "
							+ util::StringUtils::toString(static_cast<int>(predicate)));
			}
		}

	  public:
		Formatter() : defaultOptions(DefaultFormattingOptionStringEmitter<CharT>::instance) {}
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
			parseFormatString<IteratorT>(state, false, true);
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

		String format(
			const String& format,
			const Item& item0
		) const {
			const Item* array[] = {
				&item0
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(1u));
		}

		String format(
			const String& format,
			const Item& item0,
			const Item& item1
		) const {
			const Item* array[] = {
				&item0,
				&item1
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(2u));
		}

		String format(
			const String& format,
			const Item& item0,
			const Item& item1,
			const Item& item2
		) const {
			const Item* array[] = {
				&item0,
				&item1,
				&item2
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(3u));
		}

		String format(
			const String& format,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3
		) const {
			const Item* array[] = {
				&item0,
				&item1,
				&item2,
				&item3
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(4u));
		}

		String format(
			const String& format,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3,
			const Item& item4
		) const {
			const Item* array[] = {
				&item0,
				&item1,
				&item2,
				&item3,
				&item4
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(5u));
		}

		String format(
			const String& format,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3,
			const Item& item4,
			const Item& item5
		) const {
			const Item* array[] = {
				&item0,
				&item1,
				&item2,
				&item3,
				&item4,
				&item5
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(6u));
		}

		String format(
			const String& format,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3,
			const Item& item4,
			const Item& item5,
			const Item& item6
		) const {
			const Item* array[] = {
				&item0,
				&item1,
				&item2,
				&item3,
				&item4,
				&item5,
				&item6
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(7u));
		}

		String format(
			const String& format,
			const Item& item0,
			const Item& item1,
			const Item& item2,
			const Item& item3,
			const Item& item4,
			const Item& item5,
			const Item& item6,
			const Item& item7
		) const {
			const Item* array[] = {
				&item0,
				&item1,
				&item2,
				&item3,
				&item4,
				&item5,
				&item6,
				&item7
			};
			return formatIter<const Item**>(format, array, static_cast<size_t>(8u));
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTER_HPP */

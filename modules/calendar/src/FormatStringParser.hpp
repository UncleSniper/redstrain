#ifndef REDSTRAIN_MOD_CALENDAR_FORMATSTRINGPARSER_HPP
#define REDSTRAIN_MOD_CALENDAR_FORMATSTRINGPARSER_HPP

#include "FormatStringSink.hpp"
#include "DefaultFormatStringRendition.hpp"
#include "UnexpectedEndOfFormatStringError.hpp"
#include "UnexpectedFormatStringCharacterError.hpp"

namespace redengine {
namespace calendar {

	/* format_string ::= (verbatim_text | formatting)
	 * verbatim_text ::= NON_PERCENT_CHAR+
	 * formatting    ::= '%' ('%' | modifier* component)
	 * modifier      ::= width | '0' | ' '
	 * width         ::= NONZERO_DIGIT
	 * component     ::= 'Y' | 'm' | 'd' | 'H' | 'M' | 's' | 'l' | 'r'
	 *                   | 'a' [abbreviated name of the day of the week]
	 *                   | 'A' [full name of the day of the week]
	 *                   | 'b' [abbreviated month name]
	 *                   | 'B' [full month name]
	 *                   | 'C' [century number]
	 *                   | 'h' [hour on the 12-hour clock]
	 *                   | 'j' [day of the year]
	 *                   | 'p' [AM or PM]
	 *                   | 'u' [day of the week as a decimal, range 1 to 7, Monday being 1]
	 *                   | 'w' [day of the week as a decimal, range 0 to 6, Sunday being 0]
	 *                   | 'y' [year as a decimal number without a century]
	 */

	template<
		typename CharT,
		typename FormatRenditionT = DefaultFormatStringRendition<CharT>
	>
	class FormatStringParser {

	  public:
		typedef CharT Char;
		typedef FormatRenditionT FormatRendition;

	  private:
		struct State {

			const CharT* start;
			const CharT* current;
			const CharT* end;

			State(const CharT* start, const CharT* end) : start(start), current(start), end(end) {}
			State(const State& state) : start(state.start), current(state.current), end(state.end) {}

			util::MemorySize getOffset() const {
				return static_cast<util::MemorySize>(current - start);
			}

		};

	  private:
		FormatStringSink<CharT>& sink;

	  private:
		void parseFormatString(State& state) {
			const CharT* bulkStart = state.current;
			while(state.current != state.end) {
				if(*state.current == FormatRenditionT::FORMATTING_INITIATOR) {
					const CharT* prev = state.current;
					if(++state.current == state.end)
						throw UnexpectedEndOfFormatStringError(state.getOffset());
					if(*state.current == FormatRenditionT::FORMATTING_INITIATOR) {
						sink.verbatimText(bulkStart, static_cast<util::MemorySize>(state.current - bulkStart));
						++state.current;
					}
					else {
						if(bulkStart != prev)
							sink.verbatimText(bulkStart, static_cast<util::MemorySize>(prev - bulkStart));
						parseFormatting(state);
						bulkStart = state.current;
					}
				}
				else
					++state.current;
			}
			if(bulkStart != state.end)
				sink.verbatimText(bulkStart, static_cast<util::MemorySize>(state.end - bulkStart));
		}

		void parseFormatting(State& state) {
			FormattingOptions options;
			for(;;) {
				switch(*state.current) {
					case FormatRenditionT::BLANK_PADDING_MODIFIER:
						options.padding = FormattingOptions::PAD_BLANK;
						++state.current;
						break;
					case FormatRenditionT::YEAR_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_YEAR);
						++state.current;
						return;
					case FormatRenditionT::MONTH_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_MONTH);
						++state.current;
						return;
					case FormatRenditionT::DAY_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_DAY);
						++state.current;
						return;
					case FormatRenditionT::HOUR24_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_HOUR24);
						++state.current;
						return;
					case FormatRenditionT::MINUTE_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_MINUTE);
						++state.current;
						return;
					case FormatRenditionT::SECOND_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_SECOND);
						++state.current;
						return;
					case FormatRenditionT::MILLISECOND_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_MILLISECOND);
						++state.current;
						return;
					case FormatRenditionT::MICROSECOND_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_MICROSECOND);
						++state.current;
						return;
					case FormatRenditionT::SHORT_DAY_NAME_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_SHORT_DAY_NAME);
						++state.current;
						return;
					case FormatRenditionT::LONG_DAY_NAME_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_LONG_DAY_NAME);
						++state.current;
						return;
					case FormatRenditionT::SHORT_MONTH_NAME_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_SHORT_MONTH_NAME);
						++state.current;
						return;
					case FormatRenditionT::LONG_MONTH_NAME_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_LONG_MONTH_NAME);
						++state.current;
						return;
					case FormatRenditionT::CENTURY_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_CENTURY);
						++state.current;
						return;
					case FormatRenditionT::HOUR12_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_HOUR12);
						++state.current;
						return;
					case FormatRenditionT::DAY_OF_YEAR_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_DAY_OF_YEAR);
						++state.current;
						return;
					case FormatRenditionT::AM_PM_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_AM_PM);
						++state.current;
						return;
					case FormatRenditionT::DAY_OF_WEEK1_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_DAY_OF_WEEK1);
						++state.current;
						return;
					case FormatRenditionT::DAY_OF_WEEK0_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_DAY_OF_WEEK0);
						++state.current;
						return;
					case FormatRenditionT::YEAR_IN_CENTURY_COMPONENT:
						sink.formatting(options, FormatStringSink<CharT>::COMP_YEAR_IN_CENTURY);
						++state.current;
						return;
					default:
						{
							int digit = FormatRenditionT::decodeDigit(*state.current);
							if(digit < 0)
								throw UnexpectedFormatStringCharacterError(state.getOffset());
							if(digit)
								options.width = static_cast<unsigned>(digit);
							else
								options.padding = FormattingOptions::PAD_ZERO;
							++state.current;
						}
						break;
				}
				if(state.current == state.end)
					throw UnexpectedEndOfFormatStringError(state.getOffset());
			}
		}

	  public:
		FormatStringParser(FormatStringSink<CharT>& sink) : sink(sink) {}
		FormatStringParser(const FormatStringParser& parser) : sink(parser.sink) {}

		void parseFormatString(const std::basic_string<CharT>& format) {
			const CharT* chars = format.data();
			State state(chars, chars + format.length());
			parseFormatString(state);
		}

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_FORMATSTRINGPARSER_HPP */

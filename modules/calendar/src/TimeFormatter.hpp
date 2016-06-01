#ifndef REDSTRAIN_MOD_CALENDAR_TIMEFORMATTER_HPP
#define REDSTRAIN_MOD_CALENDAR_TIMEFORMATTER_HPP

#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "TimePoint.hpp"
#include "TimeStringEmitter.hpp"
#include "FormatStringParser.hpp"
#include "DefaultIntegerFormatter.hpp"
#include "DefaultFormattingRendition.hpp"

namespace redengine {
namespace calendar {

	template<
		typename CharT,
		typename NumericRenditionT = DefaultFormattingRendition<CharT>,
		typename IntegerFormatterT = DefaultIntegerFormatter<CharT>,
		typename FormatRenditionT = typename FormatStringParser<CharT>::FormatRendition
	>
	class TimeFormatter {

	  public:
		typedef CharT Char;
		typedef NumericRenditionT NumericRendition;
		typedef IntegerFormatterT IntegerFormatter;
		typedef FormatRenditionT FormatRendition;

		typedef std::basic_string<CharT> String;

	  private:
		template<CalendarPrecision Precision, CalendarModulus Modulus>
		class Sink : public FormatStringSink<CharT> {

		  public:
			typedef typename FormatStringSink<CharT>::Component Component;

		  private:
			TimeStringEmitter<CharT>& stringEmitter;
			String& result;
			const TimePoint<Precision, Modulus>& point;
			Year year;
			Month month;
			DayInMonth day;
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;

		  private:
			template<typename IntegerT>
			inline void emitInteger(IntegerT value, const FormattingOptions& options) {
				result += IntegerFormatterT::template formatInteger<IntegerT, NumericRenditionT>(value, options);
			}

		  public:
			Sink(TimeStringEmitter<CharT>& stringEmitter, String& result, const TimePoint<Precision, Modulus>& point)
					: stringEmitter(stringEmitter), result(result), point(point) {
				point.getYearMonthDay(year, month, day);
				point.getHourMinuteSecond(hour, minute, second);
			}

			Sink(const Sink& sink) : FormatStringSink<CharT>(sink), stringEmitter(sink.stringEmitter),
					result(sink.result), point(sink.point), year(sink.year), month(sink.month), day(sink.day),
					hour(sink.hour), minute(sink.minute), second(sink.second) {}

			virtual void verbatimText(const CharT* data, util::MemorySize size) {
				result.append(data, static_cast<typename String::size_type>(size));
			}

			virtual void formatting(const FormattingOptions& options, Component component) {
				switch(component) {
					case FormatStringSink<CharT>::COMP_YEAR:
						emitInteger<Year>(year, options);
						break;
					case FormatStringSink<CharT>::COMP_MONTH:
						emitInteger<Month>(static_cast<Month>(month + static_cast<Month>(1u)), options);
						break;
					case FormatStringSink<CharT>::COMP_DAY:
						emitInteger<DayInMonth>(static_cast<DayInMonth>(day + static_cast<DayInMonth>(1u)), options);
						break;
					case FormatStringSink<CharT>::COMP_HOUR24:
						emitInteger<Hour>(hour, options);
						break;
					case FormatStringSink<CharT>::COMP_MINUTE:
						emitInteger<MinuteInHour>(minute, options);
						break;
					case FormatStringSink<CharT>::COMP_SECOND:
						emitInteger<SecondInMinute>(second, options);
						break;
					case FormatStringSink<CharT>::COMP_MILLISECOND:
						emitInteger<MillisecondInSecond>(point.getMillisecond(), options);
						break;
					case FormatStringSink<CharT>::COMP_MICROSECOND:
						emitInteger<MicrosecondInMillisecond>(point.getMicrosecond(), options);
						break;
					case FormatStringSink<CharT>::COMP_SHORT_DAY_NAME:
						result += stringEmitter.emitString(static_cast<TimeStringOption>(static_cast<int>(TSO_MON)
								+ static_cast<int>(getDayInWeekMon(point.getDayInTime()))));
						break;
					case FormatStringSink<CharT>::COMP_LONG_DAY_NAME:
						result += stringEmitter.emitString(static_cast<TimeStringOption>(static_cast<int>(TSO_MONDAY)
								+ static_cast<int>(getDayInWeekMon(point.getDayInTime()))));
						break;
					case FormatStringSink<CharT>::COMP_SHORT_MONTH_NAME:
						result += stringEmitter.emitString(static_cast<TimeStringOption>(static_cast<int>(TSO_JAN)
								+ static_cast<int>(month)));
						break;
					case FormatStringSink<CharT>::COMP_LONG_MONTH_NAME:
						result += stringEmitter.emitString(static_cast<TimeStringOption>(static_cast<int>(TSO_JANUARY)
								+ static_cast<int>(month)));
						break;
					case FormatStringSink<CharT>::COMP_CENTURY:
						emitInteger<Year>(year / static_cast<Year>(100u), options);
						break;
					case FormatStringSink<CharT>::COMP_HOUR12:
						emitInteger<Hour>(static_cast<Hour>((hour + static_cast<Hour>(11u))
								% static_cast<Hour>(12) + static_cast<Hour>(1u)), options);
						break;
					case FormatStringSink<CharT>::COMP_DAY_OF_YEAR:
						emitInteger<DayInYear>(static_cast<DayInYear>(point.getDayInYear()
								+ static_cast<DayInYear>(1u)), options);
						break;
					case FormatStringSink<CharT>::COMP_AM_PM:
						result += stringEmitter.emitString(hour < static_cast<Hour>(12u) ? TSO_AM : TSO_PM);
						break;
					case FormatStringSink<CharT>::COMP_DAY_OF_WEEK1:
						emitInteger<DayInWeek>(static_cast<DayInWeek>(getDayInWeekMon(point.getDayInTime())
								+ static_cast<DayInWeek>(1u)), options);
						break;
					case FormatStringSink<CharT>::COMP_DAY_OF_WEEK0:
						emitInteger<DayInWeek>(getDayInWeekSun(point.getDayInTime()), options);
						break;
					case FormatStringSink<CharT>::COMP_YEAR_IN_CENTURY:
						emitInteger<Year>(year % static_cast<Year>(100u), options);
						break;
					default:
						throw error::ProgrammingError("Unrecognized calendar component: "
								+ util::StringUtils::toString(static_cast<int>(component)));
				}
			}

		};

	  private:
		TimeStringEmitter<CharT>& stringEmitter;

	  public:
		TimeFormatter(TimeStringEmitter<CharT>& stringEmitter) : stringEmitter(stringEmitter) {}
		TimeFormatter(const TimeFormatter& formatter) : stringEmitter(formatter.stringEmitter) {}

		inline TimeStringEmitter<CharT>& getStringEmitter() {
			return stringEmitter;
		}

		inline const TimeStringEmitter<CharT>& getStringEmitter() const {
			return stringEmitter;
		}

		template<CalendarPrecision Precision, CalendarModulus Modulus>
		String formatTime(const String& format, const TimePoint<Precision, Modulus>& point) const {
			String result;
			Sink<Precision, Modulus> sink(const_cast<TimeStringEmitter<CharT>&>(stringEmitter), result, point);
			FormatStringParser<CharT, FormatRenditionT> parser(sink);
			parser.parseFormatString(format);
			return result;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_TIMEFORMATTER_HPP */

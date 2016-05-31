#ifndef REDSTRAIN_MOD_CALENDAR_TIMEFORMATTER_HPP
#define REDSTRAIN_MOD_CALENDAR_TIMEFORMATTER_HPP

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

		  private:
			TimeStringEmitter<CharT>& stringEmitter;
			String& result;
			const TimePoint<Precision, Modulus>& point;

		  public:
			Sink(TimeStringEmitter<CharT>& stringEmitter, String& result, const TimePoint<Precision, Modulus>& point)
					: stringEmitter(stringEmitter), result(result), point(point) {}

			Sink(const Sink& sink) : FormatStringSink<CharT>(sink), stringEmitter(sink.stringEmitter),
					result(sink.result), point(sink.point) {}

			virtual void verbatimText(const CharT* data, util::MemorySize size) {
				result.append(data, static_cast<typename String::size_type>(size));
			}

			virtual void formatting(const FormattingOptions&, Component) {
				//TODO
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
			Sink sink(const_cast<TimeStringEmitter<CharT>&>(stringEmitter), result, point);
			FormatStringParser<CharT, FormatRenditionT> parser(sink);
			parser.parseFormatString(format);
			return result;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_TIMEFORMATTER_HPP */

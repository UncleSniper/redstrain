#ifndef REDSTRAIN_MOD_CALENDAR_CONVERTTIME_HPP
#define REDSTRAIN_MOD_CALENDAR_CONVERTTIME_HPP

#include "TimePoint.hpp"

namespace redengine {
namespace calendar {

	template<CalendarPrecision TargetPrecision, CalendarModulus TargetModulus>
	class ConvertTime;

	// YYYY-mm-dd
	template<>
	class ConvertTime<PREC_DAY, MOD_YEAR> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_DAY, MOD_YEAR>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_DAY, MOD_YEAR>(point.getDayInTime());
		}

	};

	// YYYY-mm-dd HH:mm
	template<>
	class ConvertTime<PREC_MINUTE, MOD_YEAR> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MINUTE, MOD_YEAR>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MINUTE, MOD_YEAR>(point.getDayInTime(), point.getMinuteInDay());
		}

	};

	// YYYY-mm-dd HH:mm:ss
	template<>
	class ConvertTime<PREC_SECOND, MOD_YEAR> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_SECOND, MOD_YEAR>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_SECOND, MOD_YEAR>(point.getDayInTime(), point.getSecondInDay());
		}

	};

	// YYYY-mm-dd HH:mm:ss.mmm
	template<>
	class ConvertTime<PREC_MILLISECOND, MOD_YEAR> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MILLISECOND, MOD_YEAR>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MILLISECOND, MOD_YEAR>(point.getDayInTime(), point.getMillisecondInDay());
		}

	};

	// YYYY-mm-dd HH:mm:ss.mmmuuu
	template<>
	class ConvertTime<PREC_MICROSECOND, MOD_YEAR> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MICROSECOND, MOD_YEAR>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MICROSECOND, MOD_YEAR>(point.getDayInTime(), point.getMicrosecondInDay());
		}

	};

	// mm-dd
	template<>
	class ConvertTime<PREC_DAY, MOD_MONTH> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_DAY, MOD_MONTH>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_DAY, MOD_MONTH>(dayInNonLeapYear(point.getDayInYear(), point.getYear()));
		}

	};

	// mm-dd HH:mm
	template<>
	class ConvertTime<PREC_MINUTE, MOD_MONTH> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MINUTE, MOD_MONTH>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MINUTE, MOD_MONTH>(dayInNonLeapYear(point.getDayInYear(), point.getYear()),
					point.getMinuteInDay());
		}

	};

	// mm-dd HH:mm:ss
	template<>
	class ConvertTime<PREC_SECOND, MOD_MONTH> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MINUTE, MOD_MONTH>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MINUTE, MOD_MONTH>(dayInNonLeapYear(point.getDayInYear(), point.getYear()),
					point.getSecondInDay());
		}

	};

	// mm-dd HH:mm:ss.mmm
	template<>
	class ConvertTime<PREC_MILLISECOND, MOD_MONTH> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MILLISECOND, MOD_MONTH>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MILLISECOND, MOD_MONTH>(dayInNonLeapYear(point.getDayInYear(), point.getYear()),
					point.getMillisecondInDay());
		}

	};

	// mm-dd HH:mm:ss.mmmuuu
	template<>
	class ConvertTime<PREC_MICROSECOND, MOD_MONTH> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MICROSECOND, MOD_MONTH>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MICROSECOND, MOD_MONTH>(dayInNonLeapYear(point.getDayInYear(), point.getYear()),
					point.getMicrosecondInDay());
		}

	};

	// HH:mm
	template<>
	class ConvertTime<PREC_MINUTE, MOD_HOUR> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MINUTE, MOD_HOUR>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MINUTE, MOD_HOUR>(point.getMinuteInDay());
		}

	};

	// HH:mm:ss
	template<>
	class ConvertTime<PREC_SECOND, MOD_HOUR> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_SECOND, MOD_HOUR>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_SECOND, MOD_HOUR>(point.getSecondInDay());
		}

	};

	// HH:mm:ss.mmm
	template<>
	class ConvertTime<PREC_MILLISECOND, MOD_HOUR> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MILLISECOND, MOD_HOUR>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MILLISECOND, MOD_HOUR>(point.getMillisecondInDay());
		}

	};

	// HH:mm:ss.mmmuuu
	template<>
	class ConvertTime<PREC_MICROSECOND, MOD_HOUR> {

	  public:
		template<CalendarPrecision SourcePrecision, CalendarModulus SourceModulus>
		static TimePoint<PREC_MICROSECOND, MOD_HOUR>
				convert(const TimePoint<SourcePrecision, SourceModulus>& point) {
			return TimePoint<PREC_MICROSECOND, MOD_HOUR>(point.getMicrosecondInDay());
		}

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_CONVERTTIME_HPP */

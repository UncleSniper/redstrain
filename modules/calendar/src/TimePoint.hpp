#ifndef REDSTRAIN_MOD_CALENDAR_TIMEPOINT_HPP
#define REDSTRAIN_MOD_CALENDAR_TIMEPOINT_HPP

#include "CalendarModulus.hpp"
#include "CalendarPrecision.hpp"
#include "CalendarComponentOutOfBoundsError.hpp"
#include "utils.hpp"

namespace redengine {
namespace calendar {

	template<CalendarPrecision Precision, CalendarModulus Modulus = MOD_YEAR>
	class TimePoint;

	// Component types:
	//   - YYYY-mm-dd -> 0..inf -> uint64_t
	//   - mm-dd -> 0..366 -> uint16_t
	//   - HH:mm -> 0..1,440 -> uint16_t
	//   - HH:mm:ss -> 0..86,400 -> uint32_t
	//   - HH:mm:ss.mmm -> 0..86,400,000 -> uint32_t
	//   - HH:mm:ss.mmmuuu -> 0..86,400,000,000 -> uint64_t

#define REDSTRAIN_CALENDAR_BAD_COMPONENT(ctype, value) \
	throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::ctype, static_cast<uint64_t>(value))

	// YYYY-mm-dd
	template<>
	class TimePoint<PREC_DAY, MOD_YEAR> {

	  private:
		DayInTime date;

	  public:
		TimePoint(DayInTime date) : date(date) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
		}

		TimePoint(Year year, Month month, DayInMonth day)
				: date(yearMonthDayInMonthToDayInTime(year, month, day)) {}

		TimePoint(const TimePoint& point) : date(point.date) {}

	};

	// YYYY-mm-dd HH:mm
	template<>
	class TimePoint<PREC_MINUTE, MOD_YEAR> {

	  private:
		DayInTime date;
		MinuteInDay time;

	  public:
		TimePoint(DayInTime date, MinuteInDay time) : date(date), time(time) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
			if(time >= static_cast<MinuteInDay>(1440u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MINUTE_IN_DAY, time);
		}

		TimePoint(DayInTime date, Hour hour, MinuteInHour minute)
				: date(date), time(static_cast<MinuteInDay>(hour) * static_cast<MinuteInDay>(60u)
				+ static_cast<MinuteInDay>(minute)) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
			if(hour >= static_cast<Hour>(24u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_HOUR, hour);
			if(minute >= static_cast<MinuteInHour>(60u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MINUTE_IN_HOUR, minute);
		}

		TimePoint(Year year, Month month, DayInMonth day, MinuteInDay time)
				: date(yearMonthDayInMonthToDayInTime(year, month, day)), time(time) {
			if(time >= static_cast<MinuteInDay>(1440u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MINUTE_IN_DAY, time);
		}

		TimePoint(Year year, Month month, DayInMonth day, Hour hour, MinuteInHour minute)
				: date(yearMonthDayInMonthToDayInTime(year, month, day)),
				time(static_cast<MinuteInDay>(hour) * static_cast<MinuteInDay>(60u)
				+ static_cast<MinuteInDay>(minute)) {
			if(hour >= static_cast<Hour>(24u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_HOUR, hour);
			if(minute >= static_cast<MinuteInHour>(60u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MINUTE_IN_HOUR, minute);
		}

		TimePoint(const TimePoint& point) : date(point.date), time(point.time) {}

	};

	// YYYY-mm-dd HH:mm:ss
	template<>
	class TimePoint<PREC_SECOND, MOD_YEAR> {

	};

	// YYYY-mm-dd HH:mm:ss.mmm
	template<>
	class TimePoint<PREC_MILLISECOND, MOD_YEAR> {

	};

	// YYYY-mm-dd HH:mm:ss.mmmuuu
	template<>
	class TimePoint<PREC_MICROSECOND, MOD_YEAR> {

	};

	// mm-dd
	template<>
	class TimePoint<PREC_DAY, MOD_MONTH> {

	};

	// mm-dd HH:mm
	template<>
	class TimePoint<PREC_MINUTE, MOD_MONTH> {

	};

	// mm-dd HH:mm:ss
	template<>
	class TimePoint<PREC_SECOND, MOD_MONTH> {

	};

	// mm-dd HH:mm:ss.mmm
	template<>
	class TimePoint<PREC_MILLISECOND, MOD_MONTH> {

	};

	// mm-dd HH:mm:ss.mmmuuu
	template<>
	class TimePoint<PREC_MICROSECOND, MOD_MONTH> {

	};

	// HH:mm
	template<>
	class TimePoint<PREC_MINUTE, MOD_HOUR> {

	};

	// HH:mm:ss
	template<>
	class TimePoint<PREC_SECOND, MOD_HOUR> {

	};

	// HH:mm:ss.mmm
	template<>
	class TimePoint<PREC_MILLISECOND, MOD_HOUR> {

	};

	// HH:mm:ss.mmmuuu
	template<>
	class TimePoint<PREC_MICROSECOND, MOD_HOUR> {

	};

#undef REDSTRAIN_CALENDAR_BAD_COMPONENT

	typedef TimePoint<PREC_DAY, MOD_YEAR> Date;
	typedef TimePoint<PREC_SECOND, MOD_YEAR> DateTime;
	typedef TimePoint<PREC_SECOND, MOD_HOUR> Time;

}}

#endif /* REDSTRAIN_MOD_CALENDAR_TIMEPOINT_HPP */

#ifndef REDSTRAIN_MOD_CALENDAR_TIMEPOINT_HPP
#define REDSTRAIN_MOD_CALENDAR_TIMEPOINT_HPP

#include "CalendarModulus.hpp"
#include "CalendarPrecision.hpp"
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

	// YYYY-mm-dd
	template<>
	class TimePoint<PREC_DAY, MOD_YEAR> {

	  private:
		DayInTime day;

	  public:
		TimePoint(DayInTime day) : day(day) {}
		TimePoint(Year year, Month month, DayInMonth day) : day(yearMonthDayInMonthToDayInTime(year, month, day)) {}

	};

	// YYYY-mm-dd HH:mm
	template<>
	class TimePoint<PREC_MINUTE, MOD_YEAR> {

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

	typedef TimePoint<PREC_DAY, MOD_YEAR> Date;
	typedef TimePoint<PREC_SECOND, MOD_YEAR> DateTime;
	typedef TimePoint<PREC_SECOND, MOD_HOUR> Time;

}}

#endif /* REDSTRAIN_MOD_CALENDAR_TIMEPOINT_HPP */

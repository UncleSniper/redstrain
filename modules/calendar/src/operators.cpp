#include "Duration.hpp"
#include "TimePoint.hpp"

namespace redengine {
namespace calendar {

#define defplus(prec, mod) \
	REDSTRAIN_CALENDAR_API TimePoint<prec, mod> \
			operator+(const TimePoint<prec, mod>& initial, const Duration& delta)

	// See the comment in Duration.cpp for why the
	// division/modulo operations here are actually
	// well-defined(-ish).

#define doPlusDate(getYMD) \
	Year year; \
	Month month; \
	DayInMonth day; \
	getYMD \
	int32_t newMonth = static_cast<int32_t>(month) + delta.getMonths(); \
	int32_t yearDelta = (newMonth < static_cast<int32_t>(0) \
			? newMonth - static_cast<int32_t>(11) : newMonth) / static_cast<int32_t>(12); \
	newMonth -= yearDelta * static_cast<int32_t>(12); \
	year = static_cast<Year>(static_cast<int32_t>(year) + delta.getYears() + yearDelta); \
	DayInMonth nodim = static_cast<DayInMonth>(numberOfDaysInMonth(static_cast<Month>(newMonth), year)); \
	if(day >= nodim) { \
		day -= nodim; \
		if(++newMonth > static_cast<int32_t>(11)) { \
			newMonth = static_cast<int32_t>(0); \
			++year; \
		} \
	} \
	DayInTime dit = yearMonthDayToDayInTime(year, static_cast<Month>(newMonth), day);

#define getYMD_YYYYmmdd initial.getYearMonthDay(year, month, day);

	// YYYY-mm-dd
	defplus(PREC_DAY, MOD_YEAR) {
		doPlusDate(getYMD_YYYYmmdd)
		return TimePoint<PREC_DAY, MOD_YEAR>(dit);
	}

	// YYYY-mm-dd HH:mm
	defplus(PREC_MINUTE, MOD_YEAR) {
		doPlusDate(getYMD_YYYYmmdd)
		int64_t newMinute = static_cast<int64_t>(initial.getMinuteInDay()) + Duration::overMinute(delta.modDay());
		int64_t dayDelta = (newMinute < static_cast<int64_t>(0)
				? newMinute - (static_cast<int64_t>(numberOfMinutesInDay) - static_cast<int64_t>(1))
				: newMinute) / static_cast<int64_t>(numberOfMinutesInDay);
		newMinute -= dayDelta * static_cast<int64_t>(numberOfMinutesInDay);
		dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + dayDelta);
		return TimePoint<PREC_MINUTE, MOD_YEAR>(dit, static_cast<MinuteInDay>(newMinute));
	}

	/*
	// YYYY-mm-dd HH:mm:ss
	defplus(PREC_SECOND, MOD_YEAR)

	// YYYY-mm-dd HH:mm:ss.mmm
	defplus(PREC_MILLISECOND, MOD_YEAR)

	// YYYY-mm-dd HH:mm:ss.mmmuuu
	defplus(PREC_MICROSECOND, MOD_YEAR)

	// mm-dd
	defplus(PREC_DAY, MOD_MONTH)

	// mm-dd HH:mm
	defplus(PREC_MINUTE, MOD_MONTH)

	// mm-dd HH:mm:ss
	defplus(PREC_SECOND, MOD_MONTH)

	// mm-dd HH:mm:ss.mmm
	defplus(PREC_MILLISECOND, MOD_MONTH)

	// mm-dd HH:mm:ss.mmmuuu
	defplus(PREC_MICROSECOND, MOD_MONTH)

	// HH:mm
	defplus(PREC_MINUTE, MOD_HOUR)

	//   - PREC_SECOND, MOD_HOUR [HH:mm:ss]
	defplus(PREC_SECOND, MOD_HOUR)

	//   - PREC_MILLISECOND, MOD_HOUR [HH:mm:ss.mmm]
	defplus(PREC_MILLISECOND, MOD_HOUR)

	//   - PREC_MICROSECOND, MOD_HOUR [HH:mm:ss.mmmuuu]
	defplus(PREC_MICROSECOND, MOD_HOUR)
	*/

}}

#include "Duration.hpp"
#include "TimePoint.hpp"

namespace redengine {
namespace calendar {

#define defplus(prec, mod) \
	REDSTRAIN_CALENDAR_API TimePoint<prec, mod> \
			operator+(const TimePoint<prec, mod>& initial, const Duration& delta)

#define defplusassign(prec, mod) \
	REDSTRAIN_CALENDAR_API TimePoint<prec, mod>& \
			operator+=(TimePoint<prec, mod>& initial, const Duration& delta)

	// See the comment in Duration.cpp for why the
	// division/modulo operations here are actually
	// well-defined(-ish).

#define decomposePlusDate_YYYYmmdd \
	Year year; \
	Month month; \
	DayInMonth day; \
	initial.getYearMonthDay(year, month, day); \
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
	DayInTime dit = yearMonthDayToDayInTime(year, static_cast<Month>(newMonth), day); \
	dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + delta.getDays());

#define decomposePlusDate_mmdd \
	Year year; \
	Month month; \
	DayInMonth day; \
	initial.getYearMonthDay(year, month, day); \
	int32_t newMonth = static_cast<int32_t>(month) + delta.getMonths(); \
	int32_t yearDelta = (newMonth < static_cast<int32_t>(0) \
			? newMonth - static_cast<int32_t>(11) : newMonth) / static_cast<int32_t>(12); \
	newMonth -= yearDelta * static_cast<int32_t>(12); \
	DayInMonth nodim = static_cast<DayInMonth>(numberOfDaysInMonth(static_cast<Month>(newMonth))); \
	if(day >= nodim) { \
		day -= nodim; \
		if(++newMonth > static_cast<int32_t>(11)) \
			newMonth = static_cast<int32_t>(0); \
	} \
	int64_t diy = static_cast<int64_t>(monthDayToDayInYear(static_cast<Month>(newMonth), day)) + delta.getDays();

#define normalizePlusDate_mmdd \
	yearDelta = (diy < static_cast<int64_t>(0) \
			? diy - static_cast<int64_t>(364) : diy) / static_cast<int64_t>(365); \
	diy -= static_cast<int64_t>(yearDelta) * static_cast<int64_t>(365);

#define applyPlusTime_HHmm \
	int64_t newMinute = static_cast<int64_t>(initial.getMinuteInDay()) + Duration::overMinute(delta.modDay()); \
	int64_t dayDelta = (newMinute < static_cast<int64_t>(0) \
			? newMinute - (static_cast<int64_t>(numberOfMinutesInDay) - static_cast<int64_t>(1)) \
			: newMinute) / static_cast<int64_t>(numberOfMinutesInDay); \
	newMinute -= dayDelta * static_cast<int64_t>(numberOfMinutesInDay);

#define applyPlusTime_HHmmss \
	int64_t newSecond = static_cast<int64_t>(initial.getSecondInDay()) + Duration::overSecond(delta.modDay()); \
	int64_t dayDelta = (newSecond < static_cast<int64_t>(0) \
			? newSecond - (static_cast<int64_t>(numberOfSecondsInDay) - static_cast<int64_t>(1)) \
			: newSecond) / static_cast<int64_t>(numberOfSecondsInDay); \
	newSecond -= dayDelta * static_cast<int64_t>(numberOfSecondsInDay); \

#define applyPlusTime_HHmmssmmm \
	int64_t newMillisecond = static_cast<int64_t>(initial.getMillisecondInDay()) \
			+ Duration::overMillisecond(delta.modDay()); \
	int64_t dayDelta = (newMillisecond < static_cast<int64_t>(0) \
			? newMillisecond - (static_cast<int64_t>(numberOfMillisecondsInDay) - static_cast<int64_t>(1)) \
			: newMillisecond) / static_cast<int64_t>(numberOfMillisecondsInDay); \
	newMillisecond -= dayDelta * static_cast<int64_t>(numberOfMillisecondsInDay);

#define applyPlusTime_HHmmssmmmuuu \
	int64_t newMicrosecond = static_cast<int64_t>(initial.getMicrosecondInDay()) + delta.modDay(); \
	int64_t dayDelta = (newMicrosecond < static_cast<int64_t>(0) \
			? newMicrosecond - (static_cast<int64_t>(numberOfMicrosecondsInDay) - static_cast<int64_t>(1)) \
			: newMicrosecond) / static_cast<int64_t>(numberOfMicrosecondsInDay); \
	newMicrosecond -= dayDelta * static_cast<int64_t>(numberOfMicrosecondsInDay);

	// YYYY-mm-dd
	defplus(PREC_DAY, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		return TimePoint<PREC_DAY, MOD_YEAR>(dit);
	}

	defplusassign(PREC_DAY, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		initial = TimePoint<PREC_DAY, MOD_YEAR>(dit);
		return initial;
	}

	// YYYY-mm-dd HH:mm
	defplus(PREC_MINUTE, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		applyPlusTime_HHmm
		dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + dayDelta);
		return TimePoint<PREC_MINUTE, MOD_YEAR>(dit, static_cast<MinuteInDay>(newMinute));
	}

	defplusassign(PREC_MINUTE, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		applyPlusTime_HHmm
		dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + dayDelta);
		initial = TimePoint<PREC_MINUTE, MOD_YEAR>(dit, static_cast<MinuteInDay>(newMinute));
		return initial;
	}

	// YYYY-mm-dd HH:mm:ss
	defplus(PREC_SECOND, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		applyPlusTime_HHmmss
		dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + dayDelta);
		return TimePoint<PREC_SECOND, MOD_YEAR>(dit, static_cast<SecondInDay>(newSecond));
	}

	defplusassign(PREC_SECOND, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		applyPlusTime_HHmmss
		dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + dayDelta);
		initial = TimePoint<PREC_SECOND, MOD_YEAR>(dit, static_cast<SecondInDay>(newSecond));
		return initial;
	}

	// YYYY-mm-dd HH:mm:ss.mmm
	defplus(PREC_MILLISECOND, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		applyPlusTime_HHmmssmmm
		dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + dayDelta);
		return TimePoint<PREC_MILLISECOND, MOD_YEAR>(dit, static_cast<MillisecondInDay>(newMillisecond));
	}

	defplusassign(PREC_MILLISECOND, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		applyPlusTime_HHmmssmmm
		dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + dayDelta);
		initial = TimePoint<PREC_MILLISECOND, MOD_YEAR>(dit, static_cast<MillisecondInDay>(newMillisecond));
		return initial;
	}

	// YYYY-mm-dd HH:mm:ss.mmmuuu
	defplus(PREC_MICROSECOND, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		applyPlusTime_HHmmssmmmuuu
		dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + dayDelta);
		return TimePoint<PREC_MICROSECOND, MOD_YEAR>(dit, static_cast<MicrosecondInDay>(newMicrosecond));
	}

	defplusassign(PREC_MICROSECOND, MOD_YEAR) {
		decomposePlusDate_YYYYmmdd
		applyPlusTime_HHmmssmmmuuu
		dit = static_cast<DayInTime>(static_cast<int64_t>(dit) + dayDelta);
		initial = TimePoint<PREC_MICROSECOND, MOD_YEAR>(dit, static_cast<MicrosecondInDay>(newMicrosecond));
		return initial;
	}

	// mm-dd
	defplus(PREC_DAY, MOD_MONTH) {
		decomposePlusDate_mmdd
		normalizePlusDate_mmdd
		return TimePoint<PREC_DAY, MOD_MONTH>(static_cast<DayInYear>(diy));
	}

	defplusassign(PREC_DAY, MOD_MONTH) {
		decomposePlusDate_mmdd
		normalizePlusDate_mmdd
		initial = TimePoint<PREC_DAY, MOD_MONTH>(static_cast<DayInYear>(diy));
		return initial;
	}

	// mm-dd HH:mm
	defplus(PREC_MINUTE, MOD_MONTH) {
		decomposePlusDate_mmdd
		applyPlusTime_HHmm
		diy += dayDelta;
		normalizePlusDate_mmdd
		return TimePoint<PREC_MINUTE, MOD_MONTH>(static_cast<DayInYear>(diy), static_cast<MinuteInDay>(newMinute));
	}

	defplusassign(PREC_MINUTE, MOD_MONTH) {
		decomposePlusDate_mmdd
		applyPlusTime_HHmm
		diy += dayDelta;
		normalizePlusDate_mmdd
		initial = TimePoint<PREC_MINUTE, MOD_MONTH>(static_cast<DayInYear>(diy),
				static_cast<MinuteInDay>(newMinute));
		return initial;
	}

	// mm-dd HH:mm:ss
	defplus(PREC_SECOND, MOD_MONTH) {
		decomposePlusDate_mmdd
		applyPlusTime_HHmmss
		diy += dayDelta;
		normalizePlusDate_mmdd
		return TimePoint<PREC_SECOND, MOD_MONTH>(static_cast<DayInYear>(diy), static_cast<SecondInDay>(newSecond));
	}

	defplusassign(PREC_SECOND, MOD_MONTH) {
		decomposePlusDate_mmdd
		applyPlusTime_HHmmss
		diy += dayDelta;
		normalizePlusDate_mmdd
		initial = TimePoint<PREC_SECOND, MOD_MONTH>(static_cast<DayInYear>(diy),
				static_cast<SecondInDay>(newSecond));
		return initial;
	}

	// mm-dd HH:mm:ss.mmm
	defplus(PREC_MILLISECOND, MOD_MONTH) {
		decomposePlusDate_mmdd
		applyPlusTime_HHmmssmmm
		diy += dayDelta;
		normalizePlusDate_mmdd
		return TimePoint<PREC_MILLISECOND, MOD_MONTH>(static_cast<DayInYear>(diy),
				static_cast<MillisecondInDay>(newMillisecond));
	}

	defplusassign(PREC_MILLISECOND, MOD_MONTH) {
		decomposePlusDate_mmdd
		applyPlusTime_HHmmssmmm
		diy += dayDelta;
		normalizePlusDate_mmdd
		initial = TimePoint<PREC_MILLISECOND, MOD_MONTH>(static_cast<DayInYear>(diy),
				static_cast<MillisecondInDay>(newMillisecond));
		return initial;
	}

	// mm-dd HH:mm:ss.mmmuuu
	defplus(PREC_MICROSECOND, MOD_MONTH) {
		decomposePlusDate_mmdd
		applyPlusTime_HHmmssmmmuuu
		diy += dayDelta;
		normalizePlusDate_mmdd
		return TimePoint<PREC_MICROSECOND, MOD_MONTH>(static_cast<DayInYear>(diy),
				static_cast<MicrosecondInDay>(newMicrosecond));
	}

	defplusassign(PREC_MICROSECOND, MOD_MONTH) {
		decomposePlusDate_mmdd
		applyPlusTime_HHmmssmmmuuu
		diy += dayDelta;
		normalizePlusDate_mmdd
		initial = TimePoint<PREC_MICROSECOND, MOD_MONTH>(static_cast<DayInYear>(diy),
				static_cast<MicrosecondInDay>(newMicrosecond));
		return initial;
	}

	// HH:mm
	defplus(PREC_MINUTE, MOD_HOUR) {
		applyPlusTime_HHmm
		return TimePoint<PREC_MINUTE, MOD_HOUR>(static_cast<MinuteInDay>(newMinute));
	}

	defplusassign(PREC_MINUTE, MOD_HOUR) {
		applyPlusTime_HHmm
		initial = TimePoint<PREC_MINUTE, MOD_HOUR>(static_cast<MinuteInDay>(newMinute));
		return initial;
	}

	// HH:mm:ss
	defplus(PREC_SECOND, MOD_HOUR) {
		applyPlusTime_HHmmss
		return TimePoint<PREC_SECOND, MOD_HOUR>(static_cast<SecondInDay>(newSecond));
	}

	defplusassign(PREC_SECOND, MOD_HOUR) {
		applyPlusTime_HHmmss
		initial = TimePoint<PREC_SECOND, MOD_HOUR>(static_cast<SecondInDay>(newSecond));
		return initial;
	}

	// HH:mm:ss.mmm
	defplus(PREC_MILLISECOND, MOD_HOUR) {
		applyPlusTime_HHmmssmmm
		return TimePoint<PREC_MILLISECOND, MOD_HOUR>(static_cast<MillisecondInDay>(newMillisecond));
	}

	defplusassign(PREC_MILLISECOND, MOD_HOUR) {
		applyPlusTime_HHmmssmmm
		initial = TimePoint<PREC_MILLISECOND, MOD_HOUR>(static_cast<MillisecondInDay>(newMillisecond));
		return initial;
	}

	// HH:mm:ss.mmmuuu
	defplus(PREC_MICROSECOND, MOD_HOUR) {
		applyPlusTime_HHmmssmmmuuu
		return TimePoint<PREC_MICROSECOND, MOD_HOUR>(static_cast<MicrosecondInDay>(newMicrosecond));
	}

	defplusassign(PREC_MICROSECOND, MOD_HOUR) {
		applyPlusTime_HHmmssmmmuuu
		initial = TimePoint<PREC_MICROSECOND, MOD_HOUR>(static_cast<MicrosecondInDay>(newMicrosecond));
		return initial;
	}

	REDSTRAIN_CALENDAR_API Duration operator-(const Duration& duration) {
		return Duration(-duration.getYears(), -duration.getMonths(), -duration.getAggregateMicroseconds());
	}

	REDSTRAIN_CALENDAR_API Duration operator+(const Duration& a, const Duration& b) {
		return Duration(a.getYears() + b.getYears(), a.getMonths() + b.getMonths(),
				a.getAggregateMicroseconds() + b.getAggregateMicroseconds());
	}

	REDSTRAIN_CALENDAR_API Duration operator-(const Duration& a, const Duration& b) {
		return Duration(a.getYears() - b.getYears(), a.getMonths() - b.getMonths(),
				a.getAggregateMicroseconds() - b.getAggregateMicroseconds());
	}

	REDSTRAIN_CALENDAR_API Duration& operator+=(Duration& a, const Duration& b) {
		a = Duration(a.getYears() + b.getYears(), a.getMonths() + b.getMonths(),
				a.getAggregateMicroseconds() + b.getAggregateMicroseconds());
		return a;
	}

	REDSTRAIN_CALENDAR_API Duration& operator-=(Duration& a, const Duration& b) {
		a = Duration(a.getYears() - b.getYears(), a.getMonths() - b.getMonths(),
				a.getAggregateMicroseconds() - b.getAggregateMicroseconds());
		return a;
	}

}}

#ifndef REDSTRAIN_MOD_CALENDAR_OPERATORS_HPP
#define REDSTRAIN_MOD_CALENDAR_OPERATORS_HPP

#include "Duration.hpp"
#include "TimePoint.hpp"

namespace redengine {
namespace calendar {

	template<
		CalendarPrecision LeftPrecision, CalendarModulus LeftModulus,
		CalendarPrecision RightPrecision, CalendarModulus RightModulus
	>
	bool operator==(
		const TimePoint<LeftPrecision, LeftModulus>& left,
		const TimePoint<RightPrecision, RightModulus>& right
	) {
		return left.getDayInTime() == right.getDayInTime()
				&& left.getMicrosecondInDay() == right.getMicrosecondInDay();
	}

	template<
		CalendarPrecision LeftPrecision, CalendarModulus LeftModulus,
		CalendarPrecision RightPrecision, CalendarModulus RightModulus
	>
	bool operator!=(
		const TimePoint<LeftPrecision, LeftModulus>& left,
		const TimePoint<RightPrecision, RightModulus>& right
	) {
		return left.getDayInTime() != right.getDayInTime()
				|| left.getMicrosecondInDay() != right.getMicrosecondInDay();
	}

	template<
		CalendarPrecision LeftPrecision, CalendarModulus LeftModulus,
		CalendarPrecision RightPrecision, CalendarModulus RightModulus
	>
	bool operator<(
		const TimePoint<LeftPrecision, LeftModulus>& left,
		const TimePoint<RightPrecision, RightModulus>& right
	) {
		DayInTime ldit = left.getDayInTime(), rdit = right.getDayInTime();
		return ldit < rdit || (ldit == rdit && left.getMicrosecondInDay() < right.getMicrosecondInDay());
	}

	template<
		CalendarPrecision LeftPrecision, CalendarModulus LeftModulus,
		CalendarPrecision RightPrecision, CalendarModulus RightModulus
	>
	bool operator<=(
		const TimePoint<LeftPrecision, LeftModulus>& left,
		const TimePoint<RightPrecision, RightModulus>& right
	) {
		DayInTime ldit = left.getDayInTime(), rdit = right.getDayInTime();
		return ldit < rdit || (ldit == rdit && left.getMicrosecondInDay() <= right.getMicrosecondInDay());
	}

	template<
		CalendarPrecision LeftPrecision, CalendarModulus LeftModulus,
		CalendarPrecision RightPrecision, CalendarModulus RightModulus
	>
	bool operator>(
		const TimePoint<LeftPrecision, LeftModulus>& left,
		const TimePoint<RightPrecision, RightModulus>& right
	) {
		DayInTime ldit = left.getDayInTime(), rdit = right.getDayInTime();
		return ldit > rdit || (ldit == rdit && left.getMicrosecondInDay() > right.getMicrosecondInDay());
	}

	template<
		CalendarPrecision LeftPrecision, CalendarModulus LeftModulus,
		CalendarPrecision RightPrecision, CalendarModulus RightModulus
	>
	bool operator>=(
		const TimePoint<LeftPrecision, LeftModulus>& left,
		const TimePoint<RightPrecision, RightModulus>& right
	) {
		DayInTime ldit = left.getDayInTime(), rdit = right.getDayInTime();
		return ldit > rdit || (ldit == rdit && left.getMicrosecondInDay() >= right.getMicrosecondInDay());
	}

	template<
		CalendarPrecision LeftPrecision, CalendarModulus LeftModulus,
		CalendarPrecision RightPrecision, CalendarModulus RightModulus
	>
	Duration exactDifference(
		const TimePoint<LeftPrecision, LeftModulus>& left,
		const TimePoint<RightPrecision, RightModulus>& right
	) {
		DayInTime ldit = left.getDayInTime(), rdit = right.getDayInTime();
		MillisecondInDay lmid = left.getMillisecondInDay(), rmid = right.getMillisecondInDay();
		int64_t days = static_cast<int64_t>(ldit) - static_cast<int64_t>(rdit);
		int64_t microseconds = static_cast<int64_t>(lmid) - static_cast<int64_t>(rmid);
		return Duration(static_cast<int32_t>(0), static_cast<int32_t>(0),
				days * static_cast<int32_t>(numberOfMicrosecondsInDay) + microseconds);
	}

	template<
		CalendarPrecision LeftPrecision, CalendarModulus LeftModulus,
		CalendarPrecision RightPrecision, CalendarModulus RightModulus
	>
	Duration symbolicDifference(
		const TimePoint<LeftPrecision, LeftModulus>& left,
		const TimePoint<RightPrecision, RightModulus>& right,
		bool carry
	) {
		Year lyear, ryear;
		Month lmonth, rmonth;
		DayInMonth lday, rday;
		left.getYearMonthDay(lyear, lmonth, lday);
		right.getYearMonthDay(ryear, rmonth, rday);
		MillisecondInDay lmid = left.getMillisecondInDay(), rmid = right.getMillisecondInDay();
		int64_t days = static_cast<int64_t>(lday) - static_cast<int64_t>(rday);
		if(carry)
			days += static_cast<int64_t>(numberOfFeb29Before(ryear, rmonth, rday))
					- static_cast<int64_t>(numberOfFeb29Before(lyear, lmonth, lday));
		int64_t microseconds = static_cast<int64_t>(lmid) - static_cast<int64_t>(rmid);
		return Duration(
			static_cast<int32_t>(lyear) - static_cast<int32_t>(ryear),
			static_cast<int32_t>(lmonth) - static_cast<int32_t>(rmonth),
			days * static_cast<int32_t>(numberOfMicrosecondsInDay) + microseconds
		);
	}

	template<
		CalendarPrecision LeftPrecision, CalendarModulus LeftModulus,
		CalendarPrecision RightPrecision, CalendarModulus RightModulus
	>
	Duration operator-(
		const TimePoint<LeftPrecision, LeftModulus>& left,
		const TimePoint<RightPrecision, RightModulus>& right
	) {
		return exactDifference<LeftPrecision, LeftModulus, RightPrecision, RightModulus>(left, right);
	}

#define REDSTRAIN_CALENDAR_PLUSOPER(prec, mod) \
	REDSTRAIN_CALENDAR_API TimePoint<prec, mod> operator+(const TimePoint<prec, mod>&, const Duration&);

	REDSTRAIN_CALENDAR_PLUSOPER(PREC_DAY, MOD_YEAR)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_MINUTE, MOD_YEAR)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_SECOND, MOD_YEAR)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_MILLISECOND, MOD_YEAR)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_MICROSECOND, MOD_YEAR)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_DAY, MOD_MONTH)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_MINUTE, MOD_MONTH)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_SECOND, MOD_MONTH)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_MILLISECOND, MOD_MONTH)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_MICROSECOND, MOD_MONTH)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_MINUTE, MOD_HOUR)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_SECOND, MOD_HOUR)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_MILLISECOND, MOD_HOUR)
	REDSTRAIN_CALENDAR_PLUSOPER(PREC_MICROSECOND, MOD_HOUR)

#undef REDSTRAIN_CALENDAR_PLUSOPER

}}

#endif /* REDSTRAIN_MOD_CALENDAR_OPERATORS_HPP */

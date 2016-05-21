#ifndef REDSTRAIN_MOD_CALENDAR_UTILS_HPP
#define REDSTRAIN_MOD_CALENDAR_UTILS_HPP

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace calendar {

	template<typename IntegerT>
	inline bool isLeapYear(IntegerT year) {
		return !(year % static_cast<IntegerT>(4u))
				&& year % static_cast<IntegerT>(100u)
				&& !(year % static_cast<IntegerT>(400u));
	}

	template<typename IntegerT>
	IntegerT numberOfLeapYearsBefore(IntegerT year) {
		if(!year)
			return static_cast<IntegerT>(0u);
		--year;
		return year / static_cast<IntegerT>(4u)
				- year / static_cast<IntegerT>(100u)
				+ year / static_cast<IntegerT>(400u)
				+ static_cast<IntegerT>(1u);  // <-- Year 0 is a leap year!
	}

	REDSTRAIN_CALENDAR_API DayInMonth numberOfDaysInMonth(Month);
	REDSTRAIN_CALENDAR_API DayInMonth numberOfDaysInMonth(Month, Year);

	REDSTRAIN_CALENDAR_API unsigned aggregateNumberOfDaysInMonth(Month);
	REDSTRAIN_CALENDAR_API unsigned aggregateNumberOfDaysInMonth(Month, Year);

	REDSTRAIN_CALENDAR_API DayInTime yearMonthDayInMonthToDayInTime(Year, Month, DayInMonth);
	REDSTRAIN_CALENDAR_API void dayInTimeToYearMonthDayInMonth(DayInTime, Year&, Month&, DayInMonth&);

}}

#endif /* REDSTRAIN_MOD_CALENDAR_UTILS_HPP */

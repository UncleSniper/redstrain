#include "utils.hpp"

namespace redengine {
namespace calendar {

	static const unsigned daysInMonth[12] = {31u, 28u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u};

	static const unsigned aggregateDaysInMonth[12]
			= {31u, 59u, 90u, 120u, 151u, 181u, 212u, 243u, 273u, 304u, 334u, 365u};

	static const unsigned aggregateDaysInMonthLeap[12]
			= {31u, 60u, 91u, 121u, 152u, 182u, 213u, 244u, 274u, 305u, 335u, 366u};

	REDSTRAIN_CALENDAR_API DayInMonth numberOfDaysInMonth(Month month) {
		return static_cast<DayInMonth>(month > static_cast<Month>(11u) ? 0u : daysInMonth[month]);
	}

	REDSTRAIN_CALENDAR_API DayInMonth numberOfDaysInMonth(Month month, Year year) {
		if(month == static_cast<Month>(1u) && isLeapYear<Year>(year))
			return static_cast<DayInMonth>(29u);
		return static_cast<DayInMonth>(month > static_cast<Month>(11u) ? 0u : daysInMonth[month]);
	}

	REDSTRAIN_CALENDAR_API DayInMonth aggregateNumberOfDaysInMonth(Month month) {
		return static_cast<DayInMonth>(month > static_cast<Month>(11u) ? 0u : aggregateDaysInMonth[month]);
	}

	REDSTRAIN_CALENDAR_API DayInMonth aggregateNumberOfDaysInMonth(Month month, Year year) {
		return static_cast<DayInMonth>(month > static_cast<Month>(11u)
				? 0u : (isLeapYear<Year>(year) ? aggregateDaysInMonthLeap : aggregateDaysInMonth)[month]);
	}

	REDSTRAIN_CALENDAR_API DayInTime yearMonthDayInMonthToDayInTime(Year year, Month month, DayInMonth day) {
		return static_cast<DayInTime>(year - static_cast<Year>(1u)) * static_cast<DayInTime>(365u)
				+ static_cast<DayInTime>(numberOfLeapYearsBefore<Year>(year))
				+ (month ? static_cast<DayInTime>(aggregateNumberOfDaysInMonth(month - static_cast<Month>(1u), year))
					: static_cast<DayInTime>(0u))
				+ static_cast<DayInTime>(day);
	}

	REDSTRAIN_CALENDAR_API void dayInTimeToYearMonthDayInMonth(DayInTime dayInTime,
			Year& year, Month& month, DayInMonth& dayInMonth) {
		// This is the tricky part. In order to get the year, we need the number of
		// leap years before it, and in order to get that, we need the year.
		// To the rescue: We can do both at the same time if we divide dayInTime
		// by 365 + 1/4 - 1/100 + 1/400 and because year 0 is a leap year, this
		// should actually work.
		//     y = n / (365 + 1/4 - 1/100 + 1/400)
		//       = n / ((365 * 400 + 100 - 4 + 1) / 400)
		//       = n * 400 / (365 * 400 + 100 - 4 + 1)
		year = static_cast<Year>(dayInTime * static_cast<DayInTime>(400u) / static_cast<DayInTime>(146097u));
		dayInTime -= static_cast<DayInTime>(year - static_cast<Year>(1u)) * static_cast<DayInTime>(365u)
				+ static_cast<DayInTime>(numberOfLeapYearsBefore<Year>(year));
		//TODO
		month = 0;
		dayInMonth = 0;
	}

}}

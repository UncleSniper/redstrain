#include "CalendarComponentOutOfBoundsError.hpp"
#include "utils.hpp"

namespace redengine {
namespace calendar {

	static const unsigned daysInMonth[12] = {31u, 28u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u};

	static const unsigned aggregateDaysInMonth[12]
			= {31u, 59u, 90u, 120u, 151u, 181u, 212u, 243u, 273u, 304u, 334u, 365u};

	static const unsigned aggregateDaysInMonthLeap[12]
			= {31u, 60u, 91u, 121u, 152u, 182u, 213u, 244u, 274u, 305u, 335u, 366u};

	static const unsigned dayToMonthLUT[365] = {
		0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
		0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 1u,
		1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u,
		1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 2u, 2u, 2u, 2u, 2u,
		2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u,
		2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u, 3u,
		3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u,
		3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u,
		4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u,
		4u, 4u, 4u, 4u, 4u, 4u, 4u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u,
		5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u,
		5u, 5u, 5u, 5u, 5u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
		6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
		6u, 6u, 6u, 6u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
		7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
		7u, 7u, 7u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u,
		8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u,
		8u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u,
		9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u,
		10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u,
		10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 11u, 11u,
		11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u,
		11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u
	};

	static const unsigned dayToMonthLUTLeap[366] = {
		0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
		0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 1u,
		1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u,
		1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 2u, 2u, 2u, 2u,
		2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u,
		2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u,
		3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u,
		3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 3u, 4u, 4u, 4u, 4u, 4u, 4u, 4u,
		4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u,
		4u, 4u, 4u, 4u, 4u, 4u, 4u, 4u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u,
		5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u, 5u,
		5u, 5u, 5u, 5u, 5u, 5u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
		6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u, 6u,
		6u, 6u, 6u, 6u, 6u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
		7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u, 7u,
		7u, 7u, 7u, 7u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u,
		8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u, 8u,
		8u, 8u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u,
		9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u, 9u,
		9u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u,
		10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 10u, 11u,
		11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u,
		11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u, 11u
	};

	REDSTRAIN_CALENDAR_API DayInMonth numberOfDaysInMonth(Month month) {
		if(month > static_cast<Month>(11u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		return static_cast<DayInMonth>(daysInMonth[month]);
	}

	REDSTRAIN_CALENDAR_API DayInMonth numberOfDaysInMonth(Month month, Year year) {
		if(month > static_cast<Month>(11u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		if(year > static_cast<Year>(100000u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_YEAR,
					static_cast<uint64_t>(year));
		if(month == static_cast<Month>(1u) && isLeapYear<Year>(year))
			return static_cast<DayInMonth>(29u);
		return static_cast<DayInMonth>(daysInMonth[month]);
	}

	REDSTRAIN_CALENDAR_API unsigned aggregateNumberOfDaysInMonth(Month month) {
		if(month > static_cast<Month>(11u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		return aggregateDaysInMonth[month];
	}

	REDSTRAIN_CALENDAR_API unsigned aggregateNumberOfDaysInMonth(Month month, Year year) {
		if(month > static_cast<Month>(11u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		if(year > static_cast<Year>(100000u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_YEAR,
					static_cast<uint64_t>(year));
		return (isLeapYear<Year>(year) ? aggregateDaysInMonthLeap : aggregateDaysInMonth)[month];
	}

	REDSTRAIN_CALENDAR_API DayInTime yearMonthDayInMonthToDayInTime(Year year, Month month, DayInMonth day) {
		if(!year || year > static_cast<Year>(100000u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_YEAR,
					static_cast<uint64_t>(year));
		if(month > static_cast<Month>(11u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		if(day >= static_cast<DayInMonth>(month == static_cast<Month>(1u) && isLeapYear<Year>(year)
				? 29u : daysInMonth[month]))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_DAY_IN_MONTH,
					static_cast<uint64_t>(day));
		return static_cast<DayInTime>(year - static_cast<Year>(1u)) * static_cast<DayInTime>(365u)
				+ static_cast<DayInTime>(numberOfLeapYearsBefore<Year>(year))
				+ (month ? static_cast<DayInTime>(aggregateNumberOfDaysInMonth(month - static_cast<Month>(1u), year))
					: static_cast<DayInTime>(0u))
				+ static_cast<DayInTime>(day);
	}

	REDSTRAIN_CALENDAR_API void dayInTimeToYearMonthDayInMonth(DayInTime dayInTime,
			Year& year, Month& month, DayInMonth& dayInMonth) {
		if(!dayInTime)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_DAY_IN_TIME,
					static_cast<uint64_t>(dayInTime));
		// Dear God in Heaven, this is garbage. Julius and Gregorius need to
		// be beaten with a stick! Whoever heard of using variable moduli in
		// a numbering system? The way humans write dates is just utterly
		// brain-damaged.
		// ------------------------------------------------------------------------
		// This is the tricky part. In order to get the year, we need the number of
		// leap years before it, and in order to get that, we need the year.
		// To the rescue: We can do both at the same time if we divide dayInTime
		// by 365 + 1/4 - 1/100 + 1/400 and because year 0 is a leap year, this
		// should actually work.
		//     y = n / (365 + 1/4 - 1/100 + 1/400)
		//       = n / ((365 * 400 + 100 - 4 + 1) / 400)
		//       = n * 400 / (365 * 400 + 100 - 4 + 1)
		year = static_cast<Year>(dayInTime * static_cast<DayInTime>(400u) / static_cast<DayInTime>(146097u))
				+ static_cast<Year>(1u);
		// Unfortunately, this approach is not entirely accurate (and I can't think
		// of a better one), as it gives the wrong (off-by-one) year in 0.077% of
		// the cases.
		//   - If the year is one too high: Since we need to decrease dayInTime by
		//     the number of days in all the years before, anyway, we can check if
		//     that delta exceeds dayInTime and adjust the year manually if needed.
		//   - If the year is one too low: The remaining dayInTime after this
		//     subtraction will exceed the day-in-year range, which we can
		//     detect and perform the wraparound manually if needed.
		DayInTime delta = static_cast<DayInTime>(year - static_cast<Year>(1u)) * static_cast<DayInTime>(365u)
				+ static_cast<DayInTime>(numberOfLeapYearsBefore<Year>(year));
		DayInTime daysThatYear;
		if(delta > dayInTime) {
			--year;
			daysThatYear = static_cast<DayInTime>(isLeapYear<Year>(year) ? 366u : 365u);
			delta -= daysThatYear;
		}
		else
			daysThatYear = static_cast<DayInTime>(isLeapYear<Year>(year) ? 366u : 365u);
		dayInTime -= delta;
		if(dayInTime >= daysThatYear) {
			++year;
			dayInTime -= daysThatYear;
		}
		month = static_cast<Month>((isLeapYear<Year>(year) ? dayToMonthLUTLeap : dayToMonthLUT)[dayInTime]);
		if(month)
			dayInTime -= static_cast<DayInTime>(aggregateNumberOfDaysInMonth(static_cast<Month>(month
					- static_cast<Month>(1u)), year));
		dayInMonth = static_cast<DayInMonth>(dayInTime);
	}

}}

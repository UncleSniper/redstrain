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
		if(month > maxMonth)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		return static_cast<DayInMonth>(daysInMonth[month]);
	}

	REDSTRAIN_CALENDAR_API DayInMonth numberOfDaysInMonth(Month month, Year year) {
		if(month > maxMonth)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		if(year > maxYear)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_YEAR,
					static_cast<uint64_t>(year));
		if(month == static_cast<Month>(1u) && isLeapYear<Year>(year))
			return static_cast<DayInMonth>(29u);
		return static_cast<DayInMonth>(daysInMonth[month]);
	}

	REDSTRAIN_CALENDAR_API unsigned aggregateNumberOfDaysInMonth(Month month) {
		if(month > maxMonth)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		return aggregateDaysInMonth[month];
	}

	REDSTRAIN_CALENDAR_API unsigned aggregateNumberOfDaysInMonth(Month month, Year year) {
		if(month > maxMonth)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		if(year > maxYear)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_YEAR,
					static_cast<uint64_t>(year));
		return (isLeapYear<Year>(year) ? aggregateDaysInMonthLeap : aggregateDaysInMonth)[month];
	}

	REDSTRAIN_CALENDAR_API DayInTime yearMonthDayToDayInTime(Year year, Month month, DayInMonth day) {
		if(!year || year > maxYear)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_YEAR,
					static_cast<uint64_t>(year));
		if(month > maxMonth)
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

	REDSTRAIN_CALENDAR_API void dayInTimeToYearMonthDay(DayInTime dayInTime,
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

	REDSTRAIN_CALENDAR_API DayInYear monthDayToDayInYear(Month month, DayInMonth day) {
		if(month > maxMonth)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		if(day >= static_cast<DayInMonth>(daysInMonth[month]))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_DAY_IN_MONTH,
					static_cast<uint64_t>(day));
		return static_cast<DayInYear>(static_cast<DayInYear>(month
				? aggregateDaysInMonth[month - static_cast<Month>(1u)] : 0u) + static_cast<DayInYear>(day));
	}

	REDSTRAIN_CALENDAR_API DayInYear monthDayToDayInYear(Month month, DayInMonth day, Year year) {
		if(!year || year > maxYear)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_YEAR,
					static_cast<uint64_t>(year));
		if(month > maxMonth)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MONTH,
					static_cast<uint64_t>(month));
		bool ly = isLeapYear<Year>(year);
		if(day >= static_cast<DayInMonth>(month == static_cast<Month>(1u) && ly ? 29u : daysInMonth[month]))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_DAY_IN_MONTH,
					static_cast<uint64_t>(day));
		return static_cast<DayInYear>(static_cast<DayInYear>(month
				? (ly ? aggregateDaysInMonthLeap : aggregateDaysInMonth)[month - static_cast<Month>(1u)] : 0u)
				+ static_cast<DayInYear>(day));
	}

	REDSTRAIN_CALENDAR_API void dayInYearToMonthDay(DayInYear dayInYear, Month& month, DayInMonth& dayInMonth) {
		if(dayInYear >= static_cast<DayInYear>(365u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_DAY_IN_YEAR,
					static_cast<uint64_t>(dayInYear));
		month = static_cast<Month>(dayToMonthLUT[dayInYear]);
		if(month)
			dayInYear -= static_cast<DayInYear>(aggregateDaysInMonth[month - static_cast<Month>(1u)]);
		dayInMonth = static_cast<DayInMonth>(dayInYear);
	}

	REDSTRAIN_CALENDAR_API void dayInYearToMonthDay(DayInYear dayInYear, Month& month, DayInMonth& dayInMonth,
			Year year) {
		if(!year || year > maxYear)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_YEAR,
					static_cast<uint64_t>(year));
		bool ly = isLeapYear<Year>(year);
		if(dayInYear >= static_cast<DayInYear>(ly ? 366u : 365u))
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_DAY_IN_YEAR,
					static_cast<uint64_t>(dayInYear));
		month = static_cast<Month>((ly ? dayToMonthLUTLeap : dayToMonthLUT)[dayInYear]);
		if(month)
			dayInYear -= static_cast<DayInYear>((ly
					? aggregateDaysInMonthLeap : aggregateDaysInMonth)[month - static_cast<Month>(1u)]);
		dayInMonth = static_cast<DayInMonth>(dayInYear);
	}

	REDSTRAIN_CALENDAR_API MinuteInDay hourMinuteToMinuteInDay(Hour hour, MinuteInHour minute) {
		if(hour >= numberOfHoursInDay)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_HOUR,
					static_cast<uint64_t>(hour));
		if(minute >= numberOfMinutesInHour)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MINUTE_IN_HOUR,
					static_cast<uint64_t>(minute));
		return static_cast<MinuteInDay>(hour) * static_cast<MinuteInDay>(60u) + static_cast<MinuteInDay>(minute);
	}

	REDSTRAIN_CALENDAR_API void minuteInDayToHourMinute(MinuteInDay minuteInDay,
			Hour& hour, MinuteInHour& minuteInHour) {
		if(minuteInDay >= numberOfMinutesInDay)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MINUTE_IN_DAY,
					static_cast<uint64_t>(minuteInDay));
		minuteInHour = static_cast<MinuteInHour>(minuteInDay % static_cast<MinuteInDay>(60u));
		hour = static_cast<Hour>(minuteInDay / static_cast<MinuteInDay>(60u));
	}

	REDSTRAIN_CALENDAR_API SecondInDay hourMinuteSecondToSecondInDay(Hour hour,
			MinuteInHour minute, SecondInMinute second) {
		if(hour >= numberOfHoursInDay)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_HOUR,
					static_cast<uint64_t>(hour));
		if(minute >= numberOfMinutesInHour)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MINUTE_IN_HOUR,
					static_cast<uint64_t>(minute));
		if(second >= numberOfSecondsInMinute)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_SECOND_IN_MINUTE,
					static_cast<uint64_t>(second));
		SecondInDay secondInDay = static_cast<SecondInDay>(hour) * static_cast<SecondInDay>(60u)
				+ static_cast<SecondInDay>(minute);
		return secondInDay * static_cast<SecondInDay>(60u) + static_cast<SecondInDay>(second);
	}

	REDSTRAIN_CALENDAR_API void secondInDayToHourMinuteSecond(SecondInDay secondInDay,
			Hour& hour, MinuteInHour& minute, SecondInMinute& secondInMinute) {
		if(secondInDay >= numberOfSecondsInDay)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_SECOND_IN_DAY,
					static_cast<uint64_t>(secondInDay));
		secondInMinute = static_cast<SecondInMinute>(secondInDay % static_cast<SecondInDay>(60u));
		secondInDay /= static_cast<SecondInDay>(60u);
		minute = static_cast<MinuteInHour>(secondInDay % static_cast<SecondInDay>(60u));
		hour = static_cast<Hour>(secondInDay / static_cast<SecondInDay>(60u));
	}

	REDSTRAIN_CALENDAR_API MillisecondInDay hourMinuteSecondMillisecondToMillisecondInDay(Hour hour,
			MinuteInHour minute, SecondInMinute second, MillisecondInSecond millisecond) {
		if(hour >= numberOfHoursInDay)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_HOUR,
					static_cast<uint64_t>(hour));
		if(minute >= numberOfMinutesInHour)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MINUTE_IN_HOUR,
					static_cast<uint64_t>(minute));
		if(second >= numberOfSecondsInMinute)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_SECOND_IN_MINUTE,
					static_cast<uint64_t>(second));
		if(millisecond >= numberOfMillisecondsInSecond)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MILLISECOND_IN_SECOND,
					static_cast<uint64_t>(millisecond));
		MillisecondInDay millisecondInDay = static_cast<MillisecondInDay>(hour) * static_cast<MillisecondInDay>(60u)
				+ static_cast<MillisecondInDay>(minute);
		millisecondInDay = millisecondInDay * static_cast<MillisecondInDay>(60u)
				+ static_cast<MillisecondInDay>(second);
		return millisecondInDay * static_cast<MillisecondInDay>(1000u)
				+ static_cast<MillisecondInDay>(millisecond);
	}

	REDSTRAIN_CALENDAR_API void millisecondInDayToHourMinuteSecondMillisecond(MillisecondInDay millisecondInDay,
			Hour& hour, MinuteInHour& minute, SecondInMinute& second, MillisecondInSecond& millisecondInSecond) {
		if(millisecondInDay >= numberOfMillisecondsInDay)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MILLISECOND_IN_DAY,
					static_cast<uint64_t>(millisecondInDay));
		millisecondInSecond = static_cast<MillisecondInSecond>(millisecondInDay
				% static_cast<MillisecondInDay>(1000u));
		millisecondInDay /= static_cast<MillisecondInDay>(1000u);
		second = static_cast<SecondInMinute>(millisecondInDay % static_cast<MillisecondInDay>(60u));
		millisecondInDay /= static_cast<MillisecondInDay>(60u);
		minute = static_cast<MinuteInHour>(millisecondInDay % static_cast<MillisecondInDay>(60u));
		hour = static_cast<Hour>(millisecondInDay / static_cast<MillisecondInDay>(60u));
	}

	REDSTRAIN_CALENDAR_API MicrosecondInDay hourMinuteSecondMillisecondMicrosecondToMicrosecondInDay(Hour hour,
			MinuteInHour minute, SecondInMinute second,
			MillisecondInSecond millisecond, MicrosecondInMillisecond microsecond) {
		if(hour >= numberOfHoursInDay)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_HOUR,
					static_cast<uint64_t>(hour));
		if(minute >= numberOfMinutesInHour)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MINUTE_IN_HOUR,
					static_cast<uint64_t>(minute));
		if(second >= numberOfSecondsInMinute)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_SECOND_IN_MINUTE,
					static_cast<uint64_t>(second));
		if(millisecond >= numberOfMillisecondsInSecond)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MILLISECOND_IN_SECOND,
					static_cast<uint64_t>(millisecond));
		if(microsecond >= numberOfMicrosecondsInMillisecond)
			throw CalendarComponentOutOfBoundsError(
					CalendarComponentOutOfBoundsError::COMP_MICROSECOND_IN_MILLISECOND,
					static_cast<uint64_t>(microsecond));
		MicrosecondInDay microsecondInDay = static_cast<MicrosecondInDay>(hour) * static_cast<MicrosecondInDay>(60u)
				+ static_cast<MicrosecondInDay>(minute);
		microsecondInDay = microsecondInDay * static_cast<MicrosecondInDay>(60u)
				+ static_cast<MicrosecondInDay>(second);
		microsecondInDay = microsecondInDay * static_cast<MicrosecondInDay>(1000u)
				+ static_cast<MicrosecondInDay>(millisecond);
		return microsecondInDay * static_cast<MicrosecondInDay>(1000u)
				+ static_cast<MicrosecondInDay>(microsecond);
	}

	REDSTRAIN_CALENDAR_API void microsecondInDayToHourMinuteSecondMillisecondMicrosecond(
			MicrosecondInDay microsecondInDay,
			Hour& hour, MinuteInHour& minute, SecondInMinute& second,
			MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecondInMillisecond) {
		if(microsecondInDay >= numberOfMicrosecondsInDay)
			throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::COMP_MICROSECOND_IN_DAY,
					static_cast<uint64_t>(microsecondInDay));
		microsecondInMillisecond = static_cast<MicrosecondInMillisecond>(microsecondInDay
				% static_cast<MicrosecondInDay>(1000u));
		microsecondInDay /= static_cast<MicrosecondInDay>(1000u);
		millisecond = static_cast<MillisecondInSecond>(microsecondInDay % static_cast<MicrosecondInDay>(1000u));
		microsecondInDay /= static_cast<MicrosecondInDay>(1000u);
		second = static_cast<SecondInMinute>(microsecondInDay % static_cast<MicrosecondInDay>(60u));
		microsecondInDay /= static_cast<MicrosecondInDay>(60u);
		minute = static_cast<MinuteInHour>(microsecondInDay % static_cast<MicrosecondInDay>(60u));
		hour = static_cast<Hour>(microsecondInDay / static_cast<MicrosecondInDay>(60u));
	}

}}

#ifndef REDSTRAIN_MOD_CALENDAR_UTILS_HPP
#define REDSTRAIN_MOD_CALENDAR_UTILS_HPP

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace calendar {

	const Month maxMonth = static_cast<Month>(11u);
	const Year maxYear = static_cast<Year>(100000u);

	const Hour numberOfHoursInDay = static_cast<Hour>(24u);
	const MinuteInHour numberOfMinutesInHour = static_cast<MinuteInHour>(60u);
	const SecondInMinute numberOfSecondsInMinute = static_cast<SecondInMinute>(60u);
	const MillisecondInSecond numberOfMillisecondsInSecond = static_cast<MillisecondInSecond>(1000u);
	const MicrosecondInMillisecond numberOfMicrosecondsInMillisecond = static_cast<MicrosecondInMillisecond>(1000u);

	const MinuteInDay numberOfMinutesInDay = static_cast<MinuteInDay>(1440u);
	const SecondInDay numberOfSecondsInDay = static_cast<SecondInDay>(86400u);
	const MillisecondInDay numberOfMillisecondsInDay = static_cast<MillisecondInDay>(86400000u);
	const MicrosecondInDay numberOfMicrosecondsInDay = static_cast<MicrosecondInDay>(86400000000ull);

	const uint16_t numberOfMillisecondsInMinute = static_cast<uint16_t>(60000u);
	const uint32_t numberOfMicrosecondsInMinute = static_cast<uint32_t>(60000000u);
	const uint32_t numberOfMicrosecondsInSecond = static_cast<uint32_t>(1000000u);

	template<typename IntegerT>
	inline bool isLeapYear(IntegerT year) {
		return !(year % static_cast<IntegerT>(4u))
				&& (year % static_cast<IntegerT>(100u)
				|| !(year % static_cast<IntegerT>(400u)));
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

	REDSTRAIN_CALENDAR_API DayInTime yearMonthDayToDayInTime(Year, Month, DayInMonth);
	REDSTRAIN_CALENDAR_API void dayInTimeToYearMonthDay(DayInTime, Year&, Month&, DayInMonth&);

	REDSTRAIN_CALENDAR_API DayInYear monthDayToDayInYear(Month, DayInMonth);
	REDSTRAIN_CALENDAR_API DayInYear monthDayToDayInYear(Month, DayInMonth, Year);
	REDSTRAIN_CALENDAR_API void dayInYearToMonthDay(DayInYear, Month&, DayInMonth&);
	REDSTRAIN_CALENDAR_API void dayInYearToMonthDay(DayInYear, Month&, DayInMonth&, Year);

	REDSTRAIN_CALENDAR_API MinuteInDay hourMinuteToMinuteInDay(Hour, MinuteInHour);
	REDSTRAIN_CALENDAR_API void minuteInDayToHourMinute(MinuteInDay, Hour&, MinuteInHour&);

	REDSTRAIN_CALENDAR_API SecondInDay hourMinuteSecondToSecondInDay(Hour, MinuteInHour, SecondInMinute);
	REDSTRAIN_CALENDAR_API void secondInDayToHourMinuteSecond(SecondInDay, Hour&, MinuteInHour&, SecondInMinute&);

	REDSTRAIN_CALENDAR_API MillisecondInDay hourMinuteSecondMillisecondToMillisecondInDay(Hour,
			MinuteInHour, SecondInMinute, MillisecondInSecond);
	REDSTRAIN_CALENDAR_API void millisecondInDayToHourMinuteSecondMillisecond(MillisecondInDay,
			Hour&, MinuteInHour&, SecondInMinute&, MillisecondInSecond&);

	REDSTRAIN_CALENDAR_API MicrosecondInDay hourMinuteSecondMillisecondMicrosecondToMicrosecondInDay(Hour,
			MinuteInHour, SecondInMinute, MillisecondInSecond, MicrosecondInMillisecond);
	REDSTRAIN_CALENDAR_API void microsecondInDayToHourMinuteSecondMillisecondMicrosecond(MicrosecondInDay,
			Hour&, MinuteInHour&, SecondInMinute&, MillisecondInSecond&, MicrosecondInMillisecond&);

}}

#endif /* REDSTRAIN_MOD_CALENDAR_UTILS_HPP */

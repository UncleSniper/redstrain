#ifndef REDSTRAIN_MOD_CALENDAR_TYPES_HPP
#define REDSTRAIN_MOD_CALENDAR_TYPES_HPP

#include <stdint.h>

namespace redengine {
namespace calendar {

	typedef uint64_t DayInTime;
	typedef uint64_t Year;
	typedef uint8_t Month;
	typedef uint8_t DayInMonth;
	typedef uint16_t DayInYear;

	typedef uint8_t Hour;
	typedef uint8_t MinuteInHour;
	typedef uint8_t SecondInMinute;
	typedef uint16_t MillisecondInSecond;
	typedef uint16_t MicrosecondInMillisecond;
	typedef uint16_t MinuteInDay;
	typedef uint32_t SecondInDay;
	typedef uint32_t MillisecondInDay;
	typedef uint64_t MicrosecondInDay;

	typedef DayInMonth Day;
	typedef MinuteInHour Minute;
	typedef SecondInMinute Second;
	typedef MillisecondInSecond Millisecond;
	typedef MicrosecondInMillisecond Microsecond;

}}

#endif /* REDSTRAIN_MOD_CALENDAR_TYPES_HPP */

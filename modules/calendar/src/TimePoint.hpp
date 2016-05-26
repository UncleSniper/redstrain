#ifndef REDSTRAIN_MOD_CALENDAR_TIMEPOINT_HPP
#define REDSTRAIN_MOD_CALENDAR_TIMEPOINT_HPP

#include "CalendarModulus.hpp"
#include "CalendarPrecision.hpp"
#include "CalendarComponentOutOfBoundsError.hpp"
#include "utils.hpp"

namespace redengine {
namespace calendar {

	template<CalendarPrecision Precision, CalendarModulus Modulus = MOD_YEAR>
	class TimePoint;

	// Valid combinations:
	//   - PREC_DAY, MOD_YEAR [YYYY-mm-dd]
	//   - PREC_MINUTE, MOD_YEAR [YYYY-mm-dd HH:mm]
	//   - PREC_SECOND, MOD_YEAR [YYYY-mm-dd HH:mm:ss]
	//   - PREC_MILLISECOND, MOD_YEAR [YYYY-mm-dd HH:mm:ss.mmm]
	//   - PREC_MICROSECOND, MOD_YEAR [YYYY-mm-dd HH:mm:ss.mmmuuu]
	//   - PREC_DAY, MOD_MONTH [mm-dd]
	//   - PREC_MINUTE, MOD_MONTH [mm-dd HH:mm]
	//   - PREC_SECOND, MOD_MONTH [mm-dd HH:mm:ss]
	//   - PREC_MILLISECOND, MOD_MONTH [mm-dd HH:mm:ss.mmm]
	//   - PREC_MICROSECOND, MOD_MONTH [mm-dd HH:mm:ss.mmmuuu]
	//   - PREC_MINUTE, MOD_HOUR [HH:mm]
	//   - PREC_SECOND, MOD_HOUR [HH:mm:ss]
	//   - PREC_MILLISECOND, MOD_HOUR [HH:mm:ss.mmm]
	//   - PREC_MICROSECOND, MOD_HOUR [HH:mm:ss.mmmuuu]

	// Component types:
	//   - YYYY-mm-dd -> 0..inf -> uint64_t
	//   - mm-dd -> 0..366 -> uint16_t
	//   - HH:mm -> 0..1,440 -> uint16_t
	//   - HH:mm:ss -> 0..86,400 -> uint32_t
	//   - HH:mm:ss.mmm -> 0..86,400,000 -> uint32_t
	//   - HH:mm:ss.mmmuuu -> 0..86,400,000,000 -> uint64_t

#define REDSTRAIN_CALENDAR_BAD_COMPONENT(ctype, value) \
	throw CalendarComponentOutOfBoundsError(CalendarComponentOutOfBoundsError::ctype, static_cast<uint64_t>(value))

	// YYYY-mm-dd
	template<>
	class TimePoint<PREC_DAY, MOD_YEAR> {

	  private:
		DayInTime date;

	  public:
		TimePoint(DayInTime date) : date(date) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
		}

		TimePoint(Year year, Month month, DayInMonth day)
				: date(yearMonthDayToDayInTime(year, month, day)) {}

		TimePoint(const TimePoint& point) : date(point.date) {}

		inline DayInTime getDayInTime() const {
			return date;
		}

		DayInYear getDayInYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return monthDayToDayInYear(month, day, year);
		}

		Year getYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return year;
		}

		Month getMonth() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			dayInTimeToYearMonthDay(date, year, month, day);
		}

		inline MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(0u);
		}

		inline SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(0u);
		}

		inline MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(0u);
		}

		inline MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(0u);
		}

		inline Hour getHour() const {
			return static_cast<Hour>(0u);
		}

		inline MinuteInHour getMinute() const {
			return static_cast<MinuteInHour>(0u);
		}

		inline SecondInMinute getSecond() const {
			return static_cast<SecondInMinute>(0u);
		}

		inline MillisecondInSecond getMillisecond() const {
			return static_cast<MillisecondInSecond>(0u);
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			hour = static_cast<Hour>(0u);
			minute = static_cast<MinuteInHour>(0u);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			hour = static_cast<Hour>(0u);
			minute = static_cast<MinuteInHour>(0u);
			second = static_cast<SecondInMinute>(0u);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			hour = static_cast<Hour>(0u);
			minute = static_cast<MinuteInHour>(0u);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			hour = static_cast<Hour>(0u);
			minute = static_cast<MinuteInHour>(0u);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// YYYY-mm-dd HH:mm
	template<>
	class TimePoint<PREC_MINUTE, MOD_YEAR> {

	  private:
		DayInTime date;
		MinuteInDay time;

	  public:
		TimePoint(DayInTime date, MinuteInDay time) : date(date), time(time) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
			if(time >= numberOfMinutesInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MINUTE_IN_DAY, time);
		}

		TimePoint(DayInTime date, Hour hour, MinuteInHour minute)
				: date(date), time(hourMinuteToMinuteInDay(hour, minute)) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
		}

		TimePoint(Year year, Month month, DayInMonth day, MinuteInDay time)
				: date(yearMonthDayToDayInTime(year, month, day)), time(time) {
			if(time >= numberOfMinutesInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MINUTE_IN_DAY, time);
		}

		TimePoint(Year year, Month month, DayInMonth day, Hour hour, MinuteInHour minute)
				: date(yearMonthDayToDayInTime(year, month, day)),
				time(hourMinuteToMinuteInDay(hour, minute)) {}

		TimePoint(const TimePoint& point) : date(point.date), time(point.time) {}

		inline DayInTime getDayInTime() const {
			return date;
		}

		DayInYear getDayInYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return monthDayToDayInYear(month, day, year);
		}

		Year getYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return year;
		}

		Month getMonth() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			dayInTimeToYearMonthDay(date, year, month, day);
		}

		inline MinuteInDay getMinuteInDay() const {
			return time;
		}

		SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(time) * static_cast<SecondInDay>(numberOfSecondsInMinute);
		}

		MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(time) * static_cast<MillisecondInDay>(numberOfMillisecondsInMinute);
		}

		MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(time) * static_cast<MicrosecondInDay>(numberOfMicrosecondsInMinute);
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			minuteInDayToHourMinute(time, hour, minute);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			minuteInDayToHourMinute(time, hour, minute);
			return minute;
		}

		inline SecondInMinute getSecond() const {
			return static_cast<SecondInMinute>(0u);
		}

		inline MillisecondInSecond getMillisecond() const {
			return static_cast<MillisecondInSecond>(0u);
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			minuteInDayToHourMinute(time, hour, minute);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			minuteInDayToHourMinute(time, hour, minute);
			second = static_cast<SecondInMinute>(0u);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			minuteInDayToHourMinute(time, hour, minute);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			minuteInDayToHourMinute(time, hour, minute);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// YYYY-mm-dd HH:mm:ss
	template<>
	class TimePoint<PREC_SECOND, MOD_YEAR> {

	  private:
		DayInTime date;
		SecondInDay time;

	  public:
		TimePoint(DayInTime date, SecondInDay time) : date(date), time(time) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
			if(time >= numberOfSecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_SECOND_IN_DAY, time);
		}

		TimePoint(Year year, Month month, DayInMonth day, Hour hour, MinuteInHour minute, SecondInMinute second)
				: date(yearMonthDayToDayInTime(year, month, day)),
				time(hourMinuteSecondToSecondInDay(hour, minute, second)) {}

		TimePoint(const TimePoint& point) : date(point.date), time(point.time) {}

		inline DayInTime getDayInTime() const {
			return date;
		}

		DayInYear getDayInYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return monthDayToDayInYear(month, day, year);
		}

		Year getYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return year;
		}

		Month getMonth() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			dayInTimeToYearMonthDay(date, year, month, day);
		}

		MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(time / static_cast<SecondInDay>(numberOfSecondsInMinute));
		}

		inline SecondInDay getSecondInDay() const {
			return time;
		}

		MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(time) * static_cast<MillisecondInDay>(numberOfMillisecondsInSecond);
		}

		MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(time) * static_cast<MicrosecondInDay>(numberOfMicrosecondsInSecond);
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			return minute;
		}

		SecondInMinute getSecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			return second;
		}

		inline MillisecondInSecond getMillisecond() const {
			return static_cast<MillisecondInSecond>(0u);
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			secondInDayToHourMinuteSecond(time, hour, minute, second);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			millisecond = static_cast<MillisecondInSecond>(0u);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			millisecond = static_cast<MillisecondInSecond>(0u);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// YYYY-mm-dd HH:mm:ss.mmm
	template<>
	class TimePoint<PREC_MILLISECOND, MOD_YEAR> {

	  private:
		DayInTime date;
		MillisecondInDay time;

	  public:
		TimePoint(DayInTime date, MillisecondInDay time) : date(date), time(time) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
			if(time >= numberOfMillisecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MILLISECOND_IN_DAY, time);
		}

		TimePoint(DayInTime date, Hour hour, MinuteInHour minute, SecondInMinute second,
				MillisecondInSecond millisecond) : date(date),
				time(hourMinuteSecondMillisecondToMillisecondInDay(hour, minute, second, millisecond)) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
		}

		TimePoint(Year year, Month month, DayInMonth day, MillisecondInDay time)
				: date(yearMonthDayToDayInTime(year, month, day)), time(time) {
			if(time >= numberOfMillisecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MILLISECOND_IN_DAY, time);
		}

		TimePoint(Year year, Month month, DayInMonth day,
				Hour hour, MinuteInHour minute, SecondInMinute second, MillisecondInSecond millisecond)
				: date(yearMonthDayToDayInTime(year, month, day)),
				time(hourMinuteSecondMillisecondToMillisecondInDay(hour, minute, second, millisecond)) {}

		TimePoint(const TimePoint& point) : date(point.date), time(point.time) {}

		inline DayInTime getDayInTime() const {
			return date;
		}

		DayInYear getDayInYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return monthDayToDayInYear(month, day, year);
		}

		Year getYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return year;
		}

		Month getMonth() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			dayInTimeToYearMonthDay(date, year, month, day);
		}

		MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(time / static_cast<MillisecondInDay>(numberOfMillisecondsInMinute));
		}

		SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(time / static_cast<MillisecondInDay>(numberOfMillisecondsInSecond));
		}

		inline MillisecondInDay getMillisecondInDay() const {
			return time;
		}

		MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(time)
					* static_cast<MicrosecondInDay>(numberOfMicrosecondsInMillisecond);
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return minute;
		}

		SecondInMinute getSecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return second;
		}

		MillisecondInSecond getMillisecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return millisecond;
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// YYYY-mm-dd HH:mm:ss.mmmuuu
	template<>
	class TimePoint<PREC_MICROSECOND, MOD_YEAR> {

	  private:
		DayInTime date;
		MicrosecondInDay time;

	  public:
		TimePoint(DayInTime date, MicrosecondInDay time) : date(date), time(time) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
			if(time >= numberOfMicrosecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MICROSECOND_IN_DAY, time);
		}

		TimePoint(DayInTime date, Hour hour, MinuteInHour minute, SecondInMinute second,
				MillisecondInSecond millisecond, MicrosecondInMillisecond microsecond) : date(date),
				time(hourMinuteSecondMillisecondMicrosecondToMicrosecondInDay(hour, minute, second,
				millisecond, microsecond)) {
			if(!date)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_TIME, date);
		}

		TimePoint(Year year, Month month, DayInMonth day, MicrosecondInDay time)
				: date(yearMonthDayToDayInTime(year, month, day)), time(time) {
			if(time >= numberOfMicrosecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MICROSECOND_IN_DAY, time);
		}

		TimePoint(Year year, Month month, DayInMonth day,
				Hour hour, MinuteInHour minute, SecondInMinute second,
				MillisecondInSecond millisecond, MicrosecondInMillisecond microsecond)
				: date(yearMonthDayToDayInTime(year, month, day)),
				time(hourMinuteSecondMillisecondMicrosecondToMicrosecondInDay(hour, minute, second,
				millisecond, microsecond)) {}

		TimePoint(const TimePoint& point) : date(point.date), time(point.time) {}

		inline DayInTime getDayInTime() const {
			return date;
		}

		DayInYear getDayInYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return monthDayToDayInYear(month, day, year);
		}

		Year getYear() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return year;
		}

		Month getMonth() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Year year;
			Month month;
			DayInMonth day;
			dayInTimeToYearMonthDay(date, year, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			dayInTimeToYearMonthDay(date, year, month, day);
		}

		MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(time / static_cast<MicrosecondInDay>(numberOfMicrosecondsInMinute));
		}

		SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(time / static_cast<MicrosecondInDay>(numberOfMicrosecondsInSecond));
		}

		MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(time
					/ static_cast<MicrosecondInDay>(numberOfMicrosecondsInMillisecond));
		}

		inline MicrosecondInDay getMicrosecondInDay() const {
			return time;
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return minute;
		}

		SecondInMinute getSecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return second;
		}

		MillisecondInSecond getMillisecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return millisecond;
		}

		MicrosecondInMillisecond getMicrosecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return microsecond;
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

	};

	// mm-dd
	template<>
	class TimePoint<PREC_DAY, MOD_MONTH> {

	  private:
		DayInYear date;

	  public:
		TimePoint(DayInYear date) : date(date) {
			if(date >= static_cast<DayInYear>(365u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_YEAR, date);
		}

		TimePoint(Month month, DayInMonth day) : date(monthDayToDayInYear(month, day)) {}

		TimePoint(const TimePoint& point) : date(point.date) {}

		DayInTime getDayInTime() const {
			return static_cast<DayInTime>(date) + static_cast<DayInTime>(1u);
		}

		inline DayInYear getDayInYear() const {
			return date;
		}

		inline Year getYear() const {
			return static_cast<Year>(1u);
		}

		Month getMonth() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			year = static_cast<Year>(1u);
			dayInYearToMonthDay(date, month, day);
		}

		inline MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(0u);
		}

		inline SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(0u);
		}

		inline MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(0u);
		}

		inline MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(0u);
		}

		inline Hour getHour() const {
			return static_cast<Hour>(0u);
		}

		inline MinuteInHour getMinute() const {
			return static_cast<MinuteInHour>(0u);
		}

		inline SecondInMinute getSecond() const {
			return static_cast<SecondInMinute>(0u);
		}

		inline MillisecondInSecond getMillisecond() const {
			return static_cast<MillisecondInSecond>(0u);
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			hour = static_cast<Hour>(0u);
			minute = static_cast<MinuteInHour>(0u);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			hour = static_cast<Hour>(0u);
			minute = static_cast<MinuteInHour>(0u);
			second = static_cast<SecondInMinute>(0u);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			hour = static_cast<Hour>(0u);
			minute = static_cast<MinuteInHour>(0u);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			hour = static_cast<Hour>(0u);
			minute = static_cast<MinuteInHour>(0u);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// mm-dd HH:mm
	template<>
	class TimePoint<PREC_MINUTE, MOD_MONTH> {

	  private:
		DayInYear date;
		MinuteInDay time;

	  public:
		TimePoint(DayInYear date, MinuteInDay time) : date(date), time(time) {
			if(date >= static_cast<DayInYear>(365u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_YEAR, date);
			if(time >= numberOfMinutesInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MINUTE_IN_DAY, time);
		}

		TimePoint(Month month, DayInMonth day, Hour hour, MinuteInHour minute)
				: date(monthDayToDayInYear(month, day)), time(hourMinuteToMinuteInDay(hour, minute)) {}

		TimePoint(const TimePoint& point) : date(point.date), time(point.time) {}

		DayInTime getDayInTime() const {
			return static_cast<DayInTime>(date) + static_cast<DayInTime>(1u);
		}

		inline DayInYear getDayInYear() const {
			return date;
		}

		inline Year getYear() const {
			return static_cast<Year>(1u);
		}

		Month getMonth() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			year = static_cast<Year>(1u);
			dayInYearToMonthDay(date, month, day);
		}

		inline MinuteInDay getMinuteInDay() const {
			return time;
		}

		SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(time) * static_cast<SecondInDay>(numberOfSecondsInMinute);
		}

		MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(time) * static_cast<MillisecondInDay>(numberOfMillisecondsInMinute);
		}

		MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(time) * static_cast<MicrosecondInDay>(numberOfMicrosecondsInMinute);
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			minuteInDayToHourMinute(time, hour, minute);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			minuteInDayToHourMinute(time, hour, minute);
			return minute;
		}

		inline SecondInMinute getSecond() const {
			return static_cast<SecondInMinute>(0u);
		}

		inline MillisecondInSecond getMillisecond() const {
			return static_cast<MillisecondInSecond>(0u);
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			minuteInDayToHourMinute(time, hour, minute);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			minuteInDayToHourMinute(time, hour, minute);
			second = static_cast<SecondInMinute>(0u);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			minuteInDayToHourMinute(time, hour, minute);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			minuteInDayToHourMinute(time, hour, minute);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// mm-dd HH:mm:ss
	template<>
	class TimePoint<PREC_SECOND, MOD_MONTH> {

	  private:
		DayInYear date;
		SecondInDay time;

	  public:
		TimePoint(DayInYear date, SecondInDay time) : date(date), time(time) {
			if(date >= static_cast<DayInYear>(365u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_YEAR, date);
			if(time >= numberOfSecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_SECOND_IN_DAY, time);
		}

		TimePoint(DayInYear date, Hour hour, MinuteInHour minute, SecondInMinute second)
				: date(date), time(hourMinuteSecondToSecondInDay(hour, minute, second)) {
			if(date >= static_cast<DayInYear>(365u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_YEAR, date);
		}

		TimePoint(Month month, DayInMonth day, SecondInDay time)
				: date(monthDayToDayInYear(month, day)), time(time) {
			if(time >= numberOfSecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_SECOND_IN_DAY, time);
		}

		TimePoint(Month month, DayInMonth day, Hour hour, MinuteInHour minute, SecondInMinute second)
				: date(monthDayToDayInYear(month, day)),
				time(hourMinuteSecondToSecondInDay(hour, minute, second)) {}

		TimePoint(const TimePoint& point) : date(point.date), time(point.time) {}

		DayInTime getDayInTime() const {
			return static_cast<DayInTime>(date) + static_cast<DayInTime>(1u);
		}

		inline DayInYear getDayInYear() const {
			return date;
		}

		inline Year getYear() const {
			return static_cast<Year>(1u);
		}

		Month getMonth() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			year = static_cast<Year>(1u);
			dayInYearToMonthDay(date, month, day);
		}

		MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(time / static_cast<SecondInDay>(numberOfSecondsInMinute));
		}

		inline SecondInDay getSecondInDay() const {
			return time;
		}

		MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(time) * static_cast<MillisecondInDay>(numberOfMillisecondsInSecond);
		}

		MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(time) * static_cast<MicrosecondInDay>(numberOfMicrosecondsInSecond);
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			return minute;
		}

		SecondInMinute getSecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			return second;
		}

		inline MillisecondInSecond getMillisecond() const {
			return static_cast<MillisecondInSecond>(0u);
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			secondInDayToHourMinuteSecond(time, hour, minute, second);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			millisecond = static_cast<MillisecondInSecond>(0u);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			millisecond = static_cast<MillisecondInSecond>(0u);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// mm-dd HH:mm:ss.mmm
	template<>
	class TimePoint<PREC_MILLISECOND, MOD_MONTH> {

	  private:
		DayInYear date;
		MillisecondInDay time;

	  public:
		TimePoint(DayInYear date, MillisecondInDay time) : date(date), time(time) {
			if(date >= static_cast<DayInYear>(365u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_YEAR, date);
			if(time >= numberOfMillisecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MILLISECOND_IN_DAY, time);
		}

		TimePoint(DayInYear date, Hour hour, MinuteInHour minute, SecondInMinute second,
				MillisecondInSecond millisecond) : date(date),
				time(hourMinuteSecondMillisecondToMillisecondInDay(hour, minute, second, millisecond)) {
			if(date >= static_cast<DayInYear>(365u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_YEAR, date);
		}

		TimePoint(Month month, DayInMonth day, MillisecondInDay time)
				: date(monthDayToDayInYear(month, day)), time(time) {
			if(time >= numberOfMillisecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MILLISECOND_IN_DAY, time);
		}

		TimePoint(Month month, DayInMonth day,
				Hour hour, MinuteInHour minute, SecondInMinute second, MillisecondInSecond millisecond)
				: date(monthDayToDayInYear(month, day)),
				time(hourMinuteSecondMillisecondToMillisecondInDay(hour, minute, second, millisecond)) {}

		TimePoint(const TimePoint& point) : date(point.date), time(point.time) {}

		DayInTime getDayInTime() const {
			return static_cast<DayInTime>(date) + static_cast<DayInTime>(1u);
		}

		inline DayInYear getDayInYear() const {
			return date;
		}

		inline Year getYear() const {
			return static_cast<Year>(1u);
		}

		Month getMonth() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			year = static_cast<Year>(1u);
			dayInYearToMonthDay(date, month, day);
		}

		MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(time / static_cast<MillisecondInDay>(numberOfMillisecondsInMinute));
		}

		SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(time / static_cast<MillisecondInDay>(numberOfMillisecondsInSecond));
		}

		inline MillisecondInDay getMillisecondInDay() const {
			return time;
		}

		MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(time)
					* static_cast<MicrosecondInDay>(numberOfMicrosecondsInMillisecond);
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return minute;
		}

		SecondInMinute getSecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return second;
		}

		MillisecondInSecond getMillisecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return millisecond;
		}

		MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// mm-dd HH:mm:ss.mmmuuu
	template<>
	class TimePoint<PREC_MICROSECOND, MOD_MONTH> {

	  private:
		DayInYear date;
		MicrosecondInDay time;

	  public:
		TimePoint(DayInYear date, MicrosecondInDay time) : date(date), time(time) {
			if(date >= static_cast<DayInYear>(365u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_YEAR, date);
			if(time >= numberOfMicrosecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MICROSECOND_IN_DAY, time);
		}

		TimePoint(DayInYear date, Hour hour, MinuteInHour minute, SecondInMinute second,
				MillisecondInSecond millisecond, MicrosecondInMillisecond microsecond) : date(date),
				time(hourMinuteSecondMillisecondMicrosecondToMicrosecondInDay(hour, minute, second,
				millisecond, microsecond)) {
			if(date >= static_cast<DayInYear>(365u))
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_DAY_IN_YEAR, date);
		}

		TimePoint(Month month, DayInMonth day, MicrosecondInDay time)
				: date(monthDayToDayInYear(month, day)), time(time) {
			if(time >= numberOfMicrosecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MICROSECOND_IN_DAY, time);
		}

		TimePoint(Month month, DayInMonth day, Hour hour, MinuteInHour minute, SecondInMinute second,
				MillisecondInSecond millisecond, MicrosecondInMillisecond microsecond)
				: date(monthDayToDayInYear(month, day)),
				time(hourMinuteSecondMillisecondMicrosecondToMicrosecondInDay(hour, minute, second,
				millisecond, microsecond)) {}

		TimePoint(const TimePoint& point) : date(point.date), time(point.time) {}

		DayInTime getDayInTime() const {
			return static_cast<DayInTime>(date) + static_cast<DayInTime>(1u);
		}

		inline DayInYear getDayInYear() const {
			return date;
		}

		inline Year getYear() const {
			return static_cast<Year>(1u);
		}

		Month getMonth() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return month;
		}

		DayInMonth getDay() const {
			Month month;
			DayInMonth day;
			dayInYearToMonthDay(date, month, day);
			return day;
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			year = static_cast<Year>(1u);
			dayInYearToMonthDay(date, month, day);
		}

		MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(time / static_cast<MicrosecondInDay>(numberOfMicrosecondsInMinute));
		}

		SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(time / static_cast<MicrosecondInDay>(numberOfMicrosecondsInSecond));
		}

		MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(time
					/ static_cast<MicrosecondInDay>(numberOfMicrosecondsInMillisecond));
		}

		inline MicrosecondInDay getMicrosecondInDay() const {
			return time;
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return minute;
		}

		SecondInMinute getSecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return second;
		}

		MillisecondInSecond getMillisecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return millisecond;
		}

		MicrosecondInMillisecond getMicrosecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return microsecond;
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

	};

	// HH:mm
	template<>
	class TimePoint<PREC_MINUTE, MOD_HOUR> {

	  private:
		MinuteInDay time;

	  public:
		TimePoint(MinuteInDay time) : time(time) {
			if(time >= numberOfMinutesInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MINUTE_IN_DAY, time);
		}

		TimePoint(Hour hour, MinuteInHour minute) : time(hourMinuteToMinuteInDay(hour, minute)) {}

		TimePoint(const TimePoint& point) : time(point.time) {}

		inline DayInTime getDayInTime() const {
			return static_cast<DayInTime>(1u);
		}

		inline DayInYear getDayInYear() const {
			return static_cast<DayInYear>(0u);
		}

		inline Year getYear() const {
			return static_cast<Year>(1u);
		}

		inline Month getMonth() const {
			return static_cast<Month>(0u);
		}

		inline DayInMonth getDay() const {
			return static_cast<DayInMonth>(0u);
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			year = static_cast<Year>(1u);
			month = static_cast<Month>(0u);
			day = static_cast<DayInMonth>(0u);
		}

		inline MinuteInDay getMinuteInDay() const {
			return time;
		}

		SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(time) * static_cast<SecondInDay>(numberOfSecondsInMinute);
		}

		MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(time) * static_cast<MillisecondInDay>(numberOfMillisecondsInMinute);
		}

		MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(time) * static_cast<MicrosecondInDay>(numberOfMicrosecondsInMinute);
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			minuteInDayToHourMinute(time, hour, minute);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			minuteInDayToHourMinute(time, hour, minute);
			return minute;
		}

		inline SecondInMinute getSecond() const {
			return static_cast<SecondInMinute>(0u);
		}

		inline MillisecondInSecond getMillisecond() const {
			return static_cast<MillisecondInSecond>(0u);
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			minuteInDayToHourMinute(time, hour, minute);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			minuteInDayToHourMinute(time, hour, minute);
			second = static_cast<SecondInMinute>(0u);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			minuteInDayToHourMinute(time, hour, minute);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			minuteInDayToHourMinute(time, hour, minute);
			second = static_cast<SecondInMinute>(0u);
			millisecond = static_cast<MillisecondInSecond>(0u);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// HH:mm:ss
	template<>
	class TimePoint<PREC_SECOND, MOD_HOUR> {

	  private:
		SecondInDay time;

	  public:
		TimePoint(SecondInDay time) : time(time) {
			if(time >= numberOfSecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_SECOND_IN_DAY, time);
		}

		TimePoint(Hour hour, MinuteInHour minute, SecondInMinute second)
				: time(hourMinuteSecondToSecondInDay(hour, minute, second)) {}

		TimePoint(const TimePoint& point) : time(point.time) {}

		inline DayInTime getDayInTime() const {
			return static_cast<DayInTime>(1u);
		}

		inline DayInYear getDayInYear() const {
			return static_cast<DayInYear>(0u);
		}

		inline Year getYear() const {
			return static_cast<Year>(1u);
		}

		inline Month getMonth() const {
			return static_cast<Month>(0u);
		}

		inline DayInMonth getDay() const {
			return static_cast<DayInMonth>(0u);
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			year = static_cast<Year>(1u);
			month = static_cast<Month>(0u);
			day = static_cast<DayInMonth>(0u);
		}

		MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(time / static_cast<SecondInDay>(numberOfSecondsInMinute));
		}

		inline SecondInDay getSecondInDay() const {
			return time;
		}

		MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(time) * static_cast<MillisecondInDay>(numberOfMillisecondsInSecond);
		}

		MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(time) * static_cast<MicrosecondInDay>(numberOfMicrosecondsInSecond);
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			return minute;
		}

		SecondInMinute getSecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			return second;
		}

		inline MillisecondInSecond getMillisecond() const {
			return static_cast<MillisecondInSecond>(0u);
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			SecondInMinute second;
			secondInDayToHourMinuteSecond(time, hour, minute, second);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			secondInDayToHourMinuteSecond(time, hour, minute, second);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			millisecond = static_cast<MillisecondInSecond>(0u);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			secondInDayToHourMinuteSecond(time, hour, minute, second);
			millisecond = static_cast<MillisecondInSecond>(0u);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// HH:mm:ss.mmm
	template<>
	class TimePoint<PREC_MILLISECOND, MOD_HOUR> {

	  private:
		MillisecondInDay time;

	  public:
		TimePoint(MillisecondInDay time) : time(time) {
			if(time >= numberOfMillisecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MILLISECOND_IN_DAY, time);
		}

		TimePoint(Hour hour, MinuteInHour minute, SecondInMinute second, MillisecondInSecond millisecond)
				: time(hourMinuteSecondMillisecondToMillisecondInDay(hour, minute, second, millisecond)) {}

		TimePoint(const TimePoint& point) : time(point.time) {}

		inline DayInTime getDayInTime() const {
			return static_cast<DayInTime>(1u);
		}

		inline DayInYear getDayInYear() const {
			return static_cast<DayInYear>(0u);
		}

		inline Year getYear() const {
			return static_cast<Year>(1u);
		}

		inline Month getMonth() const {
			return static_cast<Month>(0u);
		}

		inline DayInMonth getDay() const {
			return static_cast<DayInMonth>(0u);
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			year = static_cast<Year>(1u);
			month = static_cast<Month>(0u);
			day = static_cast<DayInMonth>(0u);
		}

		MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(time / static_cast<MillisecondInDay>(numberOfMillisecondsInMinute));
		}

		SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(time / static_cast<MillisecondInDay>(numberOfMillisecondsInSecond));
		}

		inline MillisecondInDay getMillisecondInDay() const {
			return time;
		}

		MicrosecondInDay getMicrosecondInDay() const {
			return static_cast<MicrosecondInDay>(time)
					* static_cast<MicrosecondInDay>(numberOfMicrosecondsInMillisecond);
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return minute;
		}

		SecondInMinute getSecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return second;
		}

		MillisecondInSecond getMillisecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			return millisecond;
		}

		inline MicrosecondInMillisecond getMicrosecond() const {
			return static_cast<MicrosecondInMillisecond>(0u);
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			SecondInMinute second;
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			MillisecondInSecond millisecond;
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			millisecondInDayToHourMinuteSecondMillisecond(time, hour, minute, second, millisecond);
			microsecond = static_cast<MicrosecondInMillisecond>(0u);
		}

	};

	// HH:mm:ss.mmmuuu
	template<>
	class TimePoint<PREC_MICROSECOND, MOD_HOUR> {

	  private:
		MicrosecondInDay time;

	  public:
		TimePoint(MicrosecondInDay time) : time(time) {
			if(time >= numberOfMicrosecondsInDay)
				REDSTRAIN_CALENDAR_BAD_COMPONENT(COMP_MICROSECOND_IN_DAY, time);
		}

		TimePoint(Hour hour, MinuteInHour minute, SecondInMinute second,
				MillisecondInSecond millisecond, MicrosecondInMillisecond microsecond)
				: time(hourMinuteSecondMillisecondMicrosecondToMicrosecondInDay(hour, minute, second,
				millisecond, microsecond)) {}

		TimePoint(const TimePoint& point) : time(point.time) {}

		inline DayInTime getDayInTime() const {
			return static_cast<DayInTime>(1u);
		}

		inline DayInYear getDayInYear() const {
			return static_cast<DayInYear>(0u);
		}

		inline Year getYear() const {
			return static_cast<Year>(1u);
		}

		inline Month getMonth() const {
			return static_cast<Month>(0u);
		}

		inline DayInMonth getDay() const {
			return static_cast<DayInMonth>(0u);
		}

		void getYearMonthDay(Year& year, Month& month, DayInMonth& day) const {
			year = static_cast<Year>(1u);
			month = static_cast<Month>(0u);
			day = static_cast<DayInMonth>(0u);
		}

		MinuteInDay getMinuteInDay() const {
			return static_cast<MinuteInDay>(time / static_cast<MicrosecondInDay>(numberOfMicrosecondsInMinute));
		}

		SecondInDay getSecondInDay() const {
			return static_cast<SecondInDay>(time / static_cast<MicrosecondInDay>(numberOfMicrosecondsInSecond));
		}

		MillisecondInDay getMillisecondInDay() const {
			return static_cast<MillisecondInDay>(time
					/ static_cast<MicrosecondInDay>(numberOfMicrosecondsInMillisecond));
		}

		inline MicrosecondInDay getMicrosecondInDay() const {
			return time;
		}

		Hour getHour() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return hour;
		}

		MinuteInHour getMinute() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return minute;
		}

		SecondInMinute getSecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return second;
		}

		MillisecondInSecond getMillisecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return millisecond;
		}

		MicrosecondInMillisecond getMicrosecond() const {
			Hour hour;
			MinuteInHour minute;
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
			return microsecond;
		}

		void getHourMinute(Hour& hour, MinuteInHour& minute) const {
			SecondInMinute second;
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

		void getHourMinuteSecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second) const {
			MillisecondInSecond millisecond;
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

		void getHourMinuteSecondMillisecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond) const {
			MicrosecondInMillisecond microsecond;
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

		void getHourMinuteSecondMillisecondMicrosecond(Hour& hour, MinuteInHour& minute, SecondInMinute& second,
				MillisecondInSecond& millisecond, MicrosecondInMillisecond& microsecond) const {
			microsecondInDayToHourMinuteSecondMillisecondMicrosecond(time,
					hour, minute, second, millisecond, microsecond);
		}

	};

#undef REDSTRAIN_CALENDAR_BAD_COMPONENT

	typedef TimePoint<PREC_DAY, MOD_YEAR> Date;
	typedef TimePoint<PREC_SECOND, MOD_YEAR> DateTime;
	typedef TimePoint<PREC_SECOND, MOD_HOUR> Time;

}}

#endif /* REDSTRAIN_MOD_CALENDAR_TIMEPOINT_HPP */

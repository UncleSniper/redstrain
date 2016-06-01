#ifndef REDSTRAIN_MOD_CALENDAR_DEFAULTTIMESTRINGEMITTER_HPP
#define REDSTRAIN_MOD_CALENDAR_DEFAULTTIMESTRINGEMITTER_HPP

#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/text/types.hpp>

#include "TimeStringEmitter.hpp"
#include "api.hpp"

namespace redengine {
namespace calendar {

	template<typename CharT>
	class DefaultTimeStringEmitter;

	template<>
	class DefaultTimeStringEmitter<char> : public TimeStringEmitter<char> {

	  public:
		DefaultTimeStringEmitter() {}

		DefaultTimeStringEmitter(const DefaultTimeStringEmitter& emitter) : TimeStringEmitter<char>(emitter) {}

		virtual std::string emitString(TimeStringOption option) {
			switch(option) {
				case TSO_MON:
					return "Mon";
				case TSO_TUE:
					return "Tue";
				case TSO_WED:
					return "Wed";
				case TSO_THU:
					return "Thu";
				case TSO_FRI:
					return "Fri";
				case TSO_SAT:
					return "Sat";
				case TSO_SUN:
					return "Sun";
				case TSO_MONDAY:
					return "Monday";
				case TSO_TUESDAY:
					return "Tuesday";
				case TSO_WEDNESDAY:
					return "Wednesday";
				case TSO_THURSDAY:
					return "Thursday";
				case TSO_FRIDAY:
					return "Friday";
				case TSO_SATURDAY:
					return "Saturday";
				case TSO_SUNDAY:
					return "Sunday";
				case TSO_JAN:
					return "Jan";
				case TSO_FEB:
					return "Feb";
				case TSO_MAR:
					return "Mar";
				case TSO_APR:
					return "Apr";
				case TSO_MAY_S:
					return "May";
				case TSO_JUN:
					return "Jun";
				case TSO_JUL:
					return "Jul";
				case TSO_AUG:
					return "Aug";
				case TSO_SEP:
					return "Sep";
				case TSO_OCT:
					return "Oct";
				case TSO_NOV:
					return "Nov";
				case TSO_DEC:
					return "Dec";
				case TSO_JANUARY:
					return "January";
				case TSO_FEBRUARY:
					return "February";
				case TSO_MARCH:
					return "March";
				case TSO_APRIL:
					return "April";
				case TSO_MAY:
					return "May";
				case TSO_JUNE:
					return "June";
				case TSO_JULY:
					return "July";
				case TSO_AUGUST:
					return "August";
				case TSO_SEPTEMBER:
					return "September";
				case TSO_OCTOBER:
					return "October";
				case TSO_NOVEMBER:
					return "November";
				case TSO_DECEMBER:
					return "December";
				case TSO_AM:
					return "AM";
				case TSO_PM:
					return "PM";
				default:
					throw error::ProgrammingError("Unrecognized time string option in "
							"DefaultTimeStringEmitter::emitString(): "
							+ util::StringUtils::toString(static_cast<int>(option)));
			}
		}

	  public:
		static DefaultTimeStringEmitter<char> instance;

	};

	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_MON[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_TUE[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_WED[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_THU[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_FRI[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_SAT[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_SUN[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_MONDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_TUESDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_WEDNESDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_THURSDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_FRIDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_SATURDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_SUNDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_JAN[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_FEB[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_MAR[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_APR[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_MAY_S[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_JUN[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_JUL[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_AUG[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_SEP[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_OCT[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_NOV[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_DEC[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_JANUARY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_FEBRUARY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_MARCH[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_APRIL[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_MAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_JUNE[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_JULY[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_AUGUST[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_SEPTEMBER[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_OCTOBER[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_NOVEMBER[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_DECEMBER[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_AM[];
	REDSTRAIN_CALENDAR_API extern const text::Char16 DEFAULT_TIME_OPTION_STRING16_PM[];

	template<>
	class DefaultTimeStringEmitter<text::Char16> : public TimeStringEmitter<text::Char16> {

	  public:
		DefaultTimeStringEmitter() {}

		DefaultTimeStringEmitter(const DefaultTimeStringEmitter& emitter)
				: TimeStringEmitter<text::Char16>(emitter) {}

		virtual text::String16 emitString(TimeStringOption option) {
			switch(option) {
				case TSO_MON:
					return DEFAULT_TIME_OPTION_STRING16_MON;
				case TSO_TUE:
					return DEFAULT_TIME_OPTION_STRING16_TUE;
				case TSO_WED:
					return DEFAULT_TIME_OPTION_STRING16_WED;
				case TSO_THU:
					return DEFAULT_TIME_OPTION_STRING16_THU;
				case TSO_FRI:
					return DEFAULT_TIME_OPTION_STRING16_FRI;
				case TSO_SAT:
					return DEFAULT_TIME_OPTION_STRING16_SAT;
				case TSO_SUN:
					return DEFAULT_TIME_OPTION_STRING16_SUN;
				case TSO_MONDAY:
					return DEFAULT_TIME_OPTION_STRING16_MONDAY;
				case TSO_TUESDAY:
					return DEFAULT_TIME_OPTION_STRING16_TUESDAY;
				case TSO_WEDNESDAY:
					return DEFAULT_TIME_OPTION_STRING16_WEDNESDAY;
				case TSO_THURSDAY:
					return DEFAULT_TIME_OPTION_STRING16_THURSDAY;
				case TSO_FRIDAY:
					return DEFAULT_TIME_OPTION_STRING16_FRIDAY;
				case TSO_SATURDAY:
					return DEFAULT_TIME_OPTION_STRING16_SATURDAY;
				case TSO_SUNDAY:
					return DEFAULT_TIME_OPTION_STRING16_SUNDAY;
				case TSO_JAN:
					return DEFAULT_TIME_OPTION_STRING16_JAN;
				case TSO_FEB:
					return DEFAULT_TIME_OPTION_STRING16_FEB;
				case TSO_MAR:
					return DEFAULT_TIME_OPTION_STRING16_MAR;
				case TSO_APR:
					return DEFAULT_TIME_OPTION_STRING16_APR;
				case TSO_MAY_S:
					return DEFAULT_TIME_OPTION_STRING16_MAY_S;
				case TSO_JUN:
					return DEFAULT_TIME_OPTION_STRING16_JUN;
				case TSO_JUL:
					return DEFAULT_TIME_OPTION_STRING16_JUL;
				case TSO_AUG:
					return DEFAULT_TIME_OPTION_STRING16_AUG;
				case TSO_SEP:
					return DEFAULT_TIME_OPTION_STRING16_SEP;
				case TSO_OCT:
					return DEFAULT_TIME_OPTION_STRING16_OCT;
				case TSO_NOV:
					return DEFAULT_TIME_OPTION_STRING16_NOV;
				case TSO_DEC:
					return DEFAULT_TIME_OPTION_STRING16_DEC;
				case TSO_JANUARY:
					return DEFAULT_TIME_OPTION_STRING16_JANUARY;
				case TSO_FEBRUARY:
					return DEFAULT_TIME_OPTION_STRING16_FEBRUARY;
				case TSO_MARCH:
					return DEFAULT_TIME_OPTION_STRING16_MARCH;
				case TSO_APRIL:
					return DEFAULT_TIME_OPTION_STRING16_APRIL;
				case TSO_MAY:
					return DEFAULT_TIME_OPTION_STRING16_MAY;
				case TSO_JUNE:
					return DEFAULT_TIME_OPTION_STRING16_JUNE;
				case TSO_JULY:
					return DEFAULT_TIME_OPTION_STRING16_JULY;
				case TSO_AUGUST:
					return DEFAULT_TIME_OPTION_STRING16_AUGUST;
				case TSO_SEPTEMBER:
					return DEFAULT_TIME_OPTION_STRING16_SEPTEMBER;
				case TSO_OCTOBER:
					return DEFAULT_TIME_OPTION_STRING16_OCTOBER;
				case TSO_NOVEMBER:
					return DEFAULT_TIME_OPTION_STRING16_NOVEMBER;
				case TSO_DECEMBER:
					return DEFAULT_TIME_OPTION_STRING16_DECEMBER;
				case TSO_AM:
					return DEFAULT_TIME_OPTION_STRING16_AM;
				case TSO_PM:
					return DEFAULT_TIME_OPTION_STRING16_PM;
				default:
					throw error::ProgrammingError("Unrecognized time string option in "
							"DefaultTimeStringEmitter::emitString(): "
							+ util::StringUtils::toString(static_cast<int>(option)));
			}
		}

	  public:
		static DefaultTimeStringEmitter<text::Char16> instance;

	};

	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_MON[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_TUE[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_WED[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_THU[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_FRI[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_SAT[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_SUN[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_MONDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_TUESDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_WEDNESDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_THURSDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_FRIDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_SATURDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_SUNDAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_JAN[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_FEB[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_MAR[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_APR[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_MAY_S[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_JUN[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_JUL[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_AUG[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_SEP[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_OCT[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_NOV[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_DEC[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_JANUARY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_FEBRUARY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_MARCH[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_APRIL[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_MAY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_JUNE[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_JULY[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_AUGUST[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_SEPTEMBER[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_OCTOBER[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_NOVEMBER[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_DECEMBER[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_AM[];
	REDSTRAIN_CALENDAR_API extern const text::Char32 DEFAULT_TIME_OPTION_STRING32_PM[];

	template<>
	class DefaultTimeStringEmitter<text::Char32> : public TimeStringEmitter<text::Char32> {

	  public:
		DefaultTimeStringEmitter() {}

		DefaultTimeStringEmitter(const DefaultTimeStringEmitter& emitter)
				: TimeStringEmitter<text::Char32>(emitter) {}

		virtual text::String32 emitString(TimeStringOption option) {
			switch(option) {
				case TSO_MON:
					return DEFAULT_TIME_OPTION_STRING32_MON;
				case TSO_TUE:
					return DEFAULT_TIME_OPTION_STRING32_TUE;
				case TSO_WED:
					return DEFAULT_TIME_OPTION_STRING32_WED;
				case TSO_THU:
					return DEFAULT_TIME_OPTION_STRING32_THU;
				case TSO_FRI:
					return DEFAULT_TIME_OPTION_STRING32_FRI;
				case TSO_SAT:
					return DEFAULT_TIME_OPTION_STRING32_SAT;
				case TSO_SUN:
					return DEFAULT_TIME_OPTION_STRING32_SUN;
				case TSO_MONDAY:
					return DEFAULT_TIME_OPTION_STRING32_MONDAY;
				case TSO_TUESDAY:
					return DEFAULT_TIME_OPTION_STRING32_TUESDAY;
				case TSO_WEDNESDAY:
					return DEFAULT_TIME_OPTION_STRING32_WEDNESDAY;
				case TSO_THURSDAY:
					return DEFAULT_TIME_OPTION_STRING32_THURSDAY;
				case TSO_FRIDAY:
					return DEFAULT_TIME_OPTION_STRING32_FRIDAY;
				case TSO_SATURDAY:
					return DEFAULT_TIME_OPTION_STRING32_SATURDAY;
				case TSO_SUNDAY:
					return DEFAULT_TIME_OPTION_STRING32_SUNDAY;
				case TSO_JAN:
					return DEFAULT_TIME_OPTION_STRING32_JAN;
				case TSO_FEB:
					return DEFAULT_TIME_OPTION_STRING32_FEB;
				case TSO_MAR:
					return DEFAULT_TIME_OPTION_STRING32_MAR;
				case TSO_APR:
					return DEFAULT_TIME_OPTION_STRING32_APR;
				case TSO_MAY_S:
					return DEFAULT_TIME_OPTION_STRING32_MAY_S;
				case TSO_JUN:
					return DEFAULT_TIME_OPTION_STRING32_JUN;
				case TSO_JUL:
					return DEFAULT_TIME_OPTION_STRING32_JUL;
				case TSO_AUG:
					return DEFAULT_TIME_OPTION_STRING32_AUG;
				case TSO_SEP:
					return DEFAULT_TIME_OPTION_STRING32_SEP;
				case TSO_OCT:
					return DEFAULT_TIME_OPTION_STRING32_OCT;
				case TSO_NOV:
					return DEFAULT_TIME_OPTION_STRING32_NOV;
				case TSO_DEC:
					return DEFAULT_TIME_OPTION_STRING32_DEC;
				case TSO_JANUARY:
					return DEFAULT_TIME_OPTION_STRING32_JANUARY;
				case TSO_FEBRUARY:
					return DEFAULT_TIME_OPTION_STRING32_FEBRUARY;
				case TSO_MARCH:
					return DEFAULT_TIME_OPTION_STRING32_MARCH;
				case TSO_APRIL:
					return DEFAULT_TIME_OPTION_STRING32_APRIL;
				case TSO_MAY:
					return DEFAULT_TIME_OPTION_STRING32_MAY;
				case TSO_JUNE:
					return DEFAULT_TIME_OPTION_STRING32_JUNE;
				case TSO_JULY:
					return DEFAULT_TIME_OPTION_STRING32_JULY;
				case TSO_AUGUST:
					return DEFAULT_TIME_OPTION_STRING32_AUGUST;
				case TSO_SEPTEMBER:
					return DEFAULT_TIME_OPTION_STRING32_SEPTEMBER;
				case TSO_OCTOBER:
					return DEFAULT_TIME_OPTION_STRING32_OCTOBER;
				case TSO_NOVEMBER:
					return DEFAULT_TIME_OPTION_STRING32_NOVEMBER;
				case TSO_DECEMBER:
					return DEFAULT_TIME_OPTION_STRING32_DECEMBER;
				case TSO_AM:
					return DEFAULT_TIME_OPTION_STRING32_AM;
				case TSO_PM:
					return DEFAULT_TIME_OPTION_STRING32_PM;
				default:
					throw error::ProgrammingError("Unrecognized time string option in "
							"DefaultTimeStringEmitter::emitString(): "
							+ util::StringUtils::toString(static_cast<int>(option)));
			}
		}

	  public:
		static DefaultTimeStringEmitter<text::Char32> instance;

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_DEFAULTTIMESTRINGEMITTER_HPP */

#include "Duration.hpp"
#include "utils.hpp"

namespace redengine {
namespace calendar {

	Duration::Duration(int32_t years, int32_t months, int64_t microseconds)
			: years(years), months(months), microseconds(microseconds) {}

	Duration::Duration(int32_t years, int32_t months, int64_t days, int64_t hours, int64_t minutes)
			: years(years), months(months) {
		microseconds = days * static_cast<int64_t>(24) + hours;
		microseconds = microseconds * static_cast<int64_t>(60) + minutes;
		microseconds *= static_cast<int64_t>(numberOfMicrosecondsInMinute);
	}

	Duration::Duration(int32_t years, int32_t months, int64_t days, int64_t hours, int64_t minutes, int64_t seconds)
			: years(years), months(months) {
		microseconds = days * static_cast<int64_t>(24) + hours;
		microseconds = microseconds * static_cast<int64_t>(60) + minutes;
		microseconds = microseconds * static_cast<int64_t>(60) + seconds;
		microseconds *= static_cast<int64_t>(numberOfMicrosecondsInSecond);
	}

	Duration::Duration(int32_t years, int32_t months, int64_t days,
			int64_t hours, int64_t minutes, int64_t seconds, int64_t milliseconds)
			: years(years), months(months) {
		microseconds = days * static_cast<int64_t>(24) + hours;
		microseconds = microseconds * static_cast<int64_t>(60) + minutes;
		microseconds = microseconds * static_cast<int64_t>(60) + seconds;
		microseconds = microseconds * static_cast<int64_t>(1000) + milliseconds;
		microseconds *= static_cast<int64_t>(numberOfMicrosecondsInMillisecond);
	}

	Duration::Duration(int32_t years, int32_t months, int64_t days,
			int64_t hours, int64_t minutes, int64_t seconds, int64_t milliseconds, int64_t microseconds)
			: years(years), months(months) {
		this->microseconds = days * static_cast<int64_t>(24) + hours;
		this->microseconds = this->microseconds * static_cast<int64_t>(60) + minutes;
		this->microseconds = this->microseconds * static_cast<int64_t>(60) + seconds;
		this->microseconds = this->microseconds * static_cast<int64_t>(1000) + milliseconds;
		this->microseconds = this->microseconds * static_cast<int64_t>(1000) + microseconds;
	}

	// "Gentlemen, it appears we're boned."
	//        -- Turanga Leela
	//
	// In theory, at least. The issue with "cutting out"
	// the desired component is that modulo is technically
	// undefined wrt the sign of the remainder if signed
	// integers are involved. However, with a decidedly
	// non-negative denominator, the only way for the
	// remainder to not have the same sign as the numerator
	// without violating the sanctity of n = q * d + r
	// would be for the quotient to be rounded towards
	// negative infinity rather than towards zero.
	// Surely, no sane hardware (and, by extension, no
	// sane compiler) would do this, since it would mess
	// with the required circuitry quite badly. I think.

	// Just to be sure, let's go like this:
	char _moduloSemanticsAssert[!(-1 / 2)];
	// If, by the utter insanity of the target platform,
	// the quotient is rounded towards negative infinity,
	// the dimension will expand to compile-time zero
	// and this file will not compile.

	int64_t Duration::getDays() const {
		return microseconds / static_cast<int64_t>(numberOfMicrosecondsInDay);
	}

	int64_t Duration::getHours() const {
		return microseconds / static_cast<int64_t>(numberOfMicrosecondsInHour)
				% static_cast<int64_t>(numberOfHoursInDay);
	}

	int64_t Duration::getMinutes() const {
		return microseconds / static_cast<int64_t>(numberOfMicrosecondsInMinute)
				% static_cast<int64_t>(numberOfMinutesInHour);
	}

	int64_t Duration::getSeconds() const {
		return microseconds / static_cast<int64_t>(numberOfMicrosecondsInSecond)
				% static_cast<int64_t>(numberOfSecondsInMinute);
	}

	int64_t Duration::getMilliseconds() const {
		return microseconds / static_cast<int64_t>(numberOfMicrosecondsInMillisecond)
				% static_cast<int64_t>(numberOfMillisecondsInSecond);
	}

	int64_t Duration::getMicroseconds() const {
		return microseconds % static_cast<int64_t>(numberOfMicrosecondsInMillisecond);
	}

	int64_t Duration::overMillisecond() const {
		return microseconds / static_cast<int64_t>(numberOfMicrosecondsInMillisecond);
	}

	int64_t Duration::overSecond() const {
		return microseconds / static_cast<int64_t>(numberOfMicrosecondsInSecond);
	}

	int64_t Duration::overMinute() const {
		return microseconds / static_cast<int64_t>(numberOfMicrosecondsInMinute);
	}

	int64_t Duration::overHour() const {
		return microseconds / static_cast<int64_t>(numberOfMicrosecondsInHour);
	}

	int64_t Duration::modSecond() const {
		return microseconds % static_cast<int64_t>(numberOfMicrosecondsInSecond);
	}

	int64_t Duration::modMinute() const {
		return microseconds % static_cast<int64_t>(numberOfMicrosecondsInMinute);
	}

	int64_t Duration::modHour() const {
		return microseconds % static_cast<int64_t>(numberOfMicrosecondsInHour);
	}

	int64_t Duration::modDay() const {
		return microseconds % static_cast<int64_t>(numberOfMicrosecondsInDay);
	}

}}

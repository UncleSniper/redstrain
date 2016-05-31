#ifndef REDSTRAIN_MOD_CALENDAR_DURATION_HPP
#define REDSTRAIN_MOD_CALENDAR_DURATION_HPP

#include <stdint.h>

#include "api.hpp"

namespace redengine {
namespace calendar {

	class REDSTRAIN_CALENDAR_API Duration {

	  private:
		int32_t years, months;
		int64_t microseconds;

	  public:
		// YYYY-mm uuu
		Duration(int32_t, int32_t, int64_t);
		// YYYY-mm-dd HH:mm
		Duration(int32_t, int32_t, int64_t, int64_t, int64_t);
		// YYYY-mm-dd HH:mm:ss
		Duration(int32_t, int32_t, int64_t, int64_t, int64_t, int64_t);
		// YYYY-mm-dd HH:mm:ss.mmm
		Duration(int32_t, int32_t, int64_t, int64_t, int64_t, int64_t, int64_t);
		// YYYY-mm-dd HH:mm:ss.mmmuuu
		Duration(int32_t, int32_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);

		inline int32_t getYears() const {
			return years;
		}

		inline int32_t getMonths() const {
			return months;
		}

		int64_t getDays() const;
		int64_t getHours() const;
		int64_t getMinutes() const;
		int64_t getSeconds() const;
		int64_t getMilliseconds() const;
		int64_t getMicroseconds() const;

		inline int64_t getAggregateMicroseconds() const {
			return microseconds;
		}

		int64_t overMillisecond() const;
		int64_t overSecond() const;
		int64_t overMinute() const;
		int64_t overHour() const;

		int64_t modSecond() const;
		int64_t modMinute() const;
		int64_t modHour() const;
		int64_t modDay() const;

		static int64_t overMillisecond(int64_t);
		static int64_t overSecond(int64_t);
		static int64_t overMinute(int64_t);
		static int64_t overHour(int64_t);

		static int64_t modSecond(int64_t);
		static int64_t modMinute(int64_t);
		static int64_t modHour(int64_t);
		static int64_t modDay(int64_t);

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_DURATION_HPP */

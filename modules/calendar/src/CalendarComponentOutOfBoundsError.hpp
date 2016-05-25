#ifndef REDSTRAIN_MOD_CALENDAR_CALENDARCOMPONENTOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_CALENDAR_CALENDARCOMPONENTOUTOFBOUNDSERROR_HPP

#include <stdint.h>
#include <redstrain/error/IllegalArgumentError.hpp>

#include "CalendarError.hpp"

namespace redengine {
namespace calendar {

	class REDSTRAIN_CALENDAR_API CalendarComponentOutOfBoundsError
			: public CalendarError, public error::IllegalArgumentError {

	  public:
		enum Component {
			COMP_YEAR,
			COMP_MONTH,
			COMP_DAY_IN_MONTH,
			COMP_DAY_IN_YEAR,
			COMP_DAY_IN_TIME,
			COMP_HOUR,
			COMP_MINUTE_IN_HOUR,
			COMP_SECOND_IN_MINUTE,
			COMP_MILLISECOND_IN_SECOND,
			COMP_MICROSECOND_IN_MILLISECOND,
			COMP_MINUTE_IN_DAY,
			COMP_SECOND_IN_DAY,
			COMP_MILLISECOND_IN_DAY,
			COMP_MICROSECOND_IN_DAY
		};

	  private:
		const Component component;
		const uint64_t value;

	  public:
		CalendarComponentOutOfBoundsError(Component, uint64_t);
		CalendarComponentOutOfBoundsError(const CalendarComponentOutOfBoundsError&);

		inline Component getCalendarComponent() const {
			return component;
		}

		inline uint64_t getSuppliedValue() const {
			return value;
		}

		REDSTRAIN_DECLARE_ERROR(CalendarComponentOutOfBoundsError)

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_CALENDARCOMPONENTOUTOFBOUNDSERROR_HPP */

#ifndef REDSTRAIN_MOD_CALENDAR_CALENDARERROR_HPP
#define REDSTRAIN_MOD_CALENDAR_CALENDARERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace calendar {

	class REDSTRAIN_CALENDAR_API CalendarError : public virtual error::Error {

	  public:
		CalendarError();
		CalendarError(const CalendarError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(CalendarError)

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_CALENDARERROR_HPP */

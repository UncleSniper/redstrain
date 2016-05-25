#include "CalendarComponentOutOfBoundsError.hpp"

namespace redengine {
namespace calendar {

	CalendarComponentOutOfBoundsError::CalendarComponentOutOfBoundsError(Component component, uint64_t value)
			: component(component), value(value) {}

	CalendarComponentOutOfBoundsError::CalendarComponentOutOfBoundsError(const
			CalendarComponentOutOfBoundsError& error)
			: Error(error), CalendarError(error), IllegalArgumentError(error),
			component(error.component), value(error.value) {}

	REDSTRAIN_DEFINE_ERROR(CalendarComponentOutOfBoundsError) {
		out << "Calendar ";
		switch(component) {
			case COMP_YEAR:
				out << "year";
				break;
			case COMP_MONTH:
				out << "month";
				break;
			case COMP_DAY_IN_MONTH:
				out << "day [in month]";
				break;
			case COMP_DAY_IN_YEAR:
				out << "day [in year]";
				break;
			case COMP_DAY_IN_TIME:
				out << "day [in time]";
				break;
			case COMP_HOUR:
				out << "hour";
				break;
			case COMP_MINUTE_IN_HOUR:
				out << "minute [in hour]";
				break;
			case COMP_SECOND_IN_MINUTE:
				out << "second [in minute]";
				break;
			case COMP_MILLISECOND_IN_SECOND:
				out << "millisecond [in second]";
				break;
			case COMP_MICROSECOND_IN_MILLISECOND:
				out << "microsecond [in millisecond]";
				break;
			case COMP_MINUTE_IN_DAY:
				out << "minute [in day]";
				break;
			case COMP_SECOND_IN_DAY:
				out << "second [in day]";
				break;
			case COMP_MILLISECOND_IN_DAY:
				out << "millisecond [in day]";
				break;
			case COMP_MICROSECOND_IN_DAY:
				out << "microsecond [in day]";
				break;
			default:
				out << "component";
				break;
		}
		out << " out of bounds: " << value;
	}

}}

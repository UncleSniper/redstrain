#include "CalendarError.hpp"

namespace redengine {
namespace calendar {

	CalendarError::CalendarError() {}

	CalendarError::CalendarError(const CalendarError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(CalendarError)

}}

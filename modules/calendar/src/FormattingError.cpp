#include "FormattingError.hpp"

namespace redengine {
namespace calendar {

	FormattingError::FormattingError() {}

	FormattingError::FormattingError(const FormattingError& error) : Error(error), CalendarError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(FormattingError)

}}

#ifndef REDSTRAIN_MOD_CALENDAR_FORMATTINGERROR_HPP
#define REDSTRAIN_MOD_CALENDAR_FORMATTINGERROR_HPP

#include "CalendarError.hpp"

namespace redengine {
namespace calendar {

	class REDSTRAIN_CALENDAR_API FormattingError : public CalendarError {

	  public:
		FormattingError();
		FormattingError(const FormattingError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(FormattingError)

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_FORMATTINGERROR_HPP */

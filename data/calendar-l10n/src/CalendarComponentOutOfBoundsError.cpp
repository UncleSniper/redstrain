#include <redstrain/calendar/CalendarComponentOutOfBoundsError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace calendar {

	REDSTRAIN_LOCALIZE_ERROR16(CalendarComponentOutOfBoundsError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		l10n::CalendarModuleMessageKey component;
		switch(error.getCalendarComponent()) {
#define clamp(cons) \
	case CalendarComponentOutOfBoundsError::COMP_ ## cons: \
		component = l10n::MSG_CALENDAR_COMPONENT_OUT_OF_BOUNDS_ ## cons; \
		break;
			clamp(YEAR)
			clamp(MONTH)
			clamp(DAY_IN_MONTH)
			clamp(DAY_IN_YEAR)
			clamp(DAY_IN_TIME)
			clamp(HOUR)
			clamp(MINUTE_IN_HOUR)
			clamp(SECOND_IN_MINUTE)
			clamp(MILLISECOND_IN_SECOND)
			clamp(MICROSECOND_IN_MILLISECOND)
			clamp(MINUTE_IN_DAY)
			clamp(SECOND_IN_DAY)
			clamp(MILLISECOND_IN_DAY)
			clamp(MICROSECOND_IN_DAY)
#undef clamp
			default:
				component = l10n::MSG_CALENDAR_COMPONENT_OUT_OF_BOUNDS_UNKNOWN_COMPONENT;
				break;
		}
		l10n::CalendarModuleMessageCatalog16& catalog = l10n::getDefaultCalendarModuleMessageCatalog16();
		out << catalog.formatMessage(locale, l10n::MSG_CALENDAR_COMPONENT_OUT_OF_BOUNDS,
				catalog.formatMessage(locale, component), error.getSuppliedValue());
	}

}}

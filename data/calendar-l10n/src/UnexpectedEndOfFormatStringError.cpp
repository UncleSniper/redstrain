#include <redstrain/calendar/UnexpectedEndOfFormatStringError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace calendar {

	REDSTRAIN_LOCALIZE_ERROR16(UnexpectedEndOfFormatStringError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultCalendarModuleMessageCatalog16().formatMessage(locale,
				l10n::MSG_UNEXPECTED_END_OF_FORMAT_STRING, static_cast<uint64_t>(error.getOffset()));
	}

}}

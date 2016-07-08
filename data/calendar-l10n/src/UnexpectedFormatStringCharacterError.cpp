#include <redstrain/calendar/UnexpectedFormatStringCharacterError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace calendar {

	REDSTRAIN_LOCALIZE_ERROR16(UnexpectedFormatStringCharacterError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultCalendarModuleMessageCatalog16().formatMessage(locale,
				l10n::MSG_UNEXPECTED_FORMAT_STRING_CHARACTER, static_cast<uint32_t>(error.getOffset()));
	}

}}

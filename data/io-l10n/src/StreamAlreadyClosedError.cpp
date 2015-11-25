#include <redstrain/io/StreamAlreadyClosedError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace io {

	REDSTRAIN_LOCALIZE_ERROR16(StreamAlreadyClosedError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultIOModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_STREAM_ALREADY_CLOSED_ERROR
		);
	}

}}

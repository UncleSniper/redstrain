#include <redstrain/text/Transcode.hpp>
#include <redstrain/error/NoErrorLocalizerRegisteredError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace error {

	REDSTRAIN_LOCALIZE_ERROR16(NoErrorLocalizerRegisteredError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultErrorModuleMessageCatalog16().formatMessage(locale,
				l10n::MSG_NO_ERROR_LOCALIZER_REGISTERED, Transcode::utf8ToBMP(error.getExceptionClassName()));
	}

}}

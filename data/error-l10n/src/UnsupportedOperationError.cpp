#include <redstrain/text/Transcode.hpp>
#include <redstrain/error/UnsupportedOperationError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace error {

	REDSTRAIN_LOCALIZE_ERROR16(UnsupportedOperationError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultErrorModuleMessageCatalog16().formatMessage(locale, l10n::MSG_UNSUPPORTED_OPERATION,
				Transcode::utf8ToBMP(error.getOperation()));
	}

}}

#include <redstrain/text/Transcode.hpp>
#include <redstrain/platform/FileOpenError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "FileError.hpp"
#include "catalog.hpp"
#include "oserrorl10n.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(FileOpenError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_FILE_OPEN_ERROR,
			Transcode::utf8ToBMP(error.getPath()),
			l10n::localizeFileErrorAccessDirection(error.getDirection(), locale),
			l10n::localizeOSErrorMessage16(static_cast<l10n::GeneralizedErrorCode>(error.getErrorCode()), locale)
		);
	}

}}

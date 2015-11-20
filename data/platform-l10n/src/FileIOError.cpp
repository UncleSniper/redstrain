#include <redstrain/platform/FileIOError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "FileError.hpp"
#include "catalog.hpp"
#include "oserrorl10n.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(FileIOError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_FILE_IO_ERROR,
			l10n::localizeFileErrorIODirection(error.getDirection(), locale),
			l10n::localizeOSErrorMessage16(static_cast<l10n::GeneralizedErrorCode>(error.getErrorCode()), locale)
		);
	}

}}

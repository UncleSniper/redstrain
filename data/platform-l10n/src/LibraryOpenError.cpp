#include <redstrain/platform/LibraryOpenError.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(LibraryOpenError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_LIBRARY_OPEN_ERROR,
			Transcode::utf8ToBMP(error.getSystemMessage())
		);
	}

}}

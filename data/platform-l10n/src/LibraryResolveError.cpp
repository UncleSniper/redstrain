#include <redstrain/platform/LibraryResolveError.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(LibraryResolveError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_LIBRARY_RESOLVE_ERROR,
			Transcode::utf8ToBMP(error.getSystemMessage())
		);
	}

}}

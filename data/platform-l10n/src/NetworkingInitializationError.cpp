#include <redstrain/platform/NetworkingInitializationError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"
#include "oserrorl10n.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(NetworkingInitializationError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_NETWORKING_INITIALIZATION_ERROR,
			l10n::localizeOSNetworkingErrorMessage16(static_cast<l10n::GeneralizedErrorCode>(error.getErrorCode()),
					locale)
		);
	}

}}

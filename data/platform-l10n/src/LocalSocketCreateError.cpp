#include <redstrain/platform/LocalSocketCreateError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"
#include "oserrorl10n.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(LocalSocketCreateError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_LOCAL_SOCKET_CREATE_ERROR,
			l10n::localizeOSErrorMessage16(static_cast<l10n::GeneralizedErrorCode>(error.getErrorCode()), locale)
		);
	}

}}

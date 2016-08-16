#include <redstrain/platform/DeadlockError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16_NOUSE(DeadlockError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(locale, l10n::MSG_DEADLOCK_ERROR);
	}

}}

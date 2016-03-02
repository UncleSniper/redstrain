#include <redstrain/platform/InconsistentMutexUnlockError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16_NOUSE(InconsistentMutexUnlockError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_INCONSISTENT_MUTEX_UNLOCK_ERROR
		);
	}

}}

#include <redstrain/platform/IllegalConditionLockStateError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(IllegalConditionLockStateError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		l10n::PlatformModuleMessageCatalog16& catalog = l10n::getDefaultPlatformModuleMessageCatalog16();
		l10n::PlatformModuleMessageKey opkey;
		switch(error.getOperation()) {
			#define clamp(constant) \
				case ConditionVariableOperationError::constant: \
					opkey = l10n::MSG_CONDITION_VARIABLE_OPERATION_ERROR_ ## constant; \
					break;
			clamp(CREATE)
			clamp(DESTROY)
			clamp(WAIT)
			clamp(SIGNAL)
			#undef clamp
			default:
				opkey = l10n::MSG_CONDITION_VARIABLE_OPERATION_ERROR_UNKNOWN_OPERATION;
				break;
		}
		out << catalog.formatMessage(
			locale, l10n::MSG_ILLEGAL_CONDITION_LOCK_STATE_ERROR,
			catalog.formatMessage(locale, opkey)
		);
	}

}}
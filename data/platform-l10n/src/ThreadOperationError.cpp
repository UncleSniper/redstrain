#include <redstrain/platform/ThreadOperationError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"
#include "oserrorl10n.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(ThreadOperationError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		l10n::PlatformModuleMessageCatalog16& catalog = l10n::getDefaultPlatformModuleMessageCatalog16();
		l10n::PlatformModuleMessageKey opkey;
		switch(error.getOperation()) {
			#define clamp(constant) \
				case ThreadOperationError::constant: \
					opkey = l10n::MSG_THREAD_OPERATION_ERROR_ ## constant; \
					break;
			clamp(START)
			clamp(STOP)
			clamp(INTERRUPT)
			clamp(JOIN)
			#undef clamp
			default:
				opkey = l10n::MSG_THREAD_OPERATION_ERROR_UNKNOWN_OPERATION;
				break;
		}
		out << catalog.formatMessage(
			locale, l10n::MSG_ILLEGAL_THREAD_STATE_ERROR,
			catalog.formatMessage(locale, opkey),
			l10n::localizeOSErrorMessage16(static_cast<l10n::GeneralizedErrorCode>(error.getErrorCode()), locale)
		);
	}

}}

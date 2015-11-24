#include <redstrain/platform/IllegalThreadStateError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(IllegalThreadStateError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		l10n::PlatformModuleMessageCatalog16& catalog = l10n::getDefaultPlatformModuleMessageCatalog16();
		l10n::PlatformModuleMessageKey opkey, statekey;
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
		switch(error.getThreadState()) {
			#define clamp(constant) \
				case Thread::constant: \
					statekey = l10n::MSG_ILLEGAL_THREAD_STATE_ERROR_STATE_ ## constant; \
					break;
			clamp(PRISTINE)
			clamp(JOINABLE)
			clamp(DETACHED)
			clamp(TERMINATED)
			#undef clamp
			default:
				statekey = l10n::MSG_ILLEGAL_THREAD_STATE_ERROR_STATE_UNKNOWN_STATE;
				break;
		}
		out << catalog.formatMessage(
			locale, l10n::MSG_ILLEGAL_THREAD_STATE_ERROR,
			catalog.formatMessage(locale, opkey),
			catalog.formatMessage(locale, statekey)
		);
	}

}}

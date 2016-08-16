#include <redstrain/error/UncaughtExceptionError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace error {

	REDSTRAIN_LOCALIZE_ERROR16_NOUSE(UncaughtExceptionError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultErrorModuleMessageCatalog16().formatMessage(locale, l10n::MSG_UNCAUGHT_EXCEPTION);
	}

}}

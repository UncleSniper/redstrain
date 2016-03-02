#include <redstrain/error/IllegalArgumentError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace error {

	REDSTRAIN_LOCALIZE_ERROR16_NOUSE(IllegalArgumentError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultErrorModuleMessageCatalog16().formatMessage(locale, l10n::MSG_ILLEGAL_ARGUMENT);
	}

}}

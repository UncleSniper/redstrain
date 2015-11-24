#include <redstrain/platform/IllegalStandardStreamSpecifierError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(IllegalStandardStreamSpecifierError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_ILLEGAL_STANDARD_STREAM_SPECIFIER_ERROR,
			static_cast<int32_t>(error.getStreamSpecifier())
		);
	}

}}

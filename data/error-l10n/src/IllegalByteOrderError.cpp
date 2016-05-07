#include <redstrain/error/IllegalByteOrderError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace error {

	REDSTRAIN_LOCALIZE_ERROR16(IllegalByteOrderError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultErrorModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_ILLEGAL_BYTE_ORDER,
			static_cast<int32_t>(error.getByteOrder())
		);
	}

}}

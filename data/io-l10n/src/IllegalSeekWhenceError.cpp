#include <redstrain/io/IllegalSeekWhenceError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace io {

	REDSTRAIN_LOCALIZE_ERROR16(IllegalSeekWhenceError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultIOModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_ILLEGAL_SEEK_WHENCE_ERROR,
			static_cast<int32_t>(error.getSeekWhence())
		);
	}

}}

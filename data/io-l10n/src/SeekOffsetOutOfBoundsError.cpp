#include <redstrain/io/SeekOffsetOutOfBoundsError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace io {

	REDSTRAIN_LOCALIZE_ERROR16(SeekOffsetOutOfBoundsError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultIOModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_SEEK_OFFSET_OUT_OF_BOUNDS_ERROR,
			static_cast<int64_t>(error.getTargetOffset())
		);
	}

}}

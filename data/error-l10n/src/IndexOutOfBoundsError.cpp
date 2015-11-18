#include <redstrain/error/IndexOutOfBoundsError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace error {

	REDSTRAIN_LOCALIZE_ERROR16(IndexOutOfBoundsError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultErrorModuleMessageCatalog16().formatMessage(locale, l10n::MSG_INDEX_OUT_OF_BOUNDS,
				static_cast<uint64_t>(error.getOffendingIndex()));
	}

}}

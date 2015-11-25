#include <redstrain/text/Transcode.hpp>
#include <redstrain/io/NumberAddressSpaceError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace io {

	REDSTRAIN_LOCALIZE_ERROR16(NumberAddressSpaceError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultIOModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_NUMBER_ADDRESS_SPACE_ERROR,
			Transcode::utf8ToBMP(error.getOffendingRendition())
		);
	}

}}

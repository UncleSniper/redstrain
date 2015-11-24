#include <redstrain/platform/InvalidNetworkAddressError.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(InvalidNetworkAddressError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_INVALID_NETWORK_ADDRESS_ERROR,
			Transcode::utf8ToBMP(error.getAddress())
		);
	}

}}

#include <redstrain/platform/SocketPacketSizeExceededError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16_NOUSE(SocketPacketSizeExceededError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_SOCKET_PACKET_SIZE_EXCEEDED_ERROR
		);
	}

}}

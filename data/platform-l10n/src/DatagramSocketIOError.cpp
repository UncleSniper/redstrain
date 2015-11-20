#include <redstrain/platform/DatagramSocketIOError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "SocketIOError.hpp"
#include "catalog.hpp"
#include "oserrorl10n.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_LOCALIZE_ERROR16(DatagramSocketIOError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_DATAGRAM_SOCKET_IO_ERROR,
			l10n::localizeSocketIOErrorDirection(error.getDirection(), locale),
			l10n::localizeOSErrorMessage16(static_cast<l10n::GeneralizedErrorCode>(error.getErrorCode()), locale)
		);
	}

}}

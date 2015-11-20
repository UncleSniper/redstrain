#include "SocketIOError.hpp"
#include "catalog.hpp"

using redengine::text::String16;
using redengine::locale::Locale;

namespace redengine {
namespace platform {
namespace l10n {

	REDSTRAIN_PLATFORM_L10N_API String16 localizeSocketIOErrorDirection(SocketIOError::Direction direction,
			const Locale& locale) {
		l10n::PlatformModuleMessageCatalog16& catalog = l10n::getDefaultPlatformModuleMessageCatalog16();
		l10n::PlatformModuleMessageKey dirkey;
		switch(direction) {
			#define clamp(constant) \
				case SocketIOError::constant: \
					dirkey = l10n::MSG_SOCKET_IO_ERROR_ ## constant;
					break;
			clamp(SEND)
			clamp(RECEIVE)
			#undef clamp
			default:
				dirkey = l10n::MSG_SOCKET_IO_ERROR_UNKNOWN_DIRECTION;
				break;
		}
		return catalog.formatMessage(
			locale, l10n::MSG_SOCKET_IO_ERROR_DIRECTION,
			catalog.formatMessage(locale, dirkey)
		);
	}

}}}

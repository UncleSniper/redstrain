#include "FileError.hpp"
#include "catalog.hpp"

using redengine::text::String16;
using redengine::locale::Locale;

namespace redengine {
namespace platform {
namespace l10n {

	REDSTRAIN_PLATFORM_L10N_API String16 localizeFileErrorAccessDirection(File::Direction direction,
			const Locale& locale) {
		l10n::PlatformModuleMessageKey dirkey;
		switch(direction) {
			#define clamp(constant) \
				case File::constant: \
					dirkey = l10n::MSG_FILE_ERROR_ACCESS_ ## constant; \
					break;
			clamp(INPUT)
			clamp(OUTPUT)
			clamp(RANDOM_ACCESS)
			#undef clamp
			default:
				dirkey = l10n::MSG_FILE_ERROR_ACCESS_UNKNOWN_DIRECTION;
				break;
		}
		return l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(locale, dirkey);
	}

	REDSTRAIN_PLATFORM_L10N_API String16 localizeFileErrorIODirection(File::Direction direction,
			const Locale& locale) {
		l10n::PlatformModuleMessageKey dirkey;
		switch(direction) {
			#define clamp(constant) \
				case File::constant: \
					dirkey = l10n::MSG_FILE_ERROR_IO_ ## constant; \
					break;
			clamp(INPUT)
			clamp(OUTPUT)
			clamp(RANDOM_ACCESS)
			#undef clamp
			default:
				dirkey = l10n::MSG_FILE_ERROR_IO_UNKNOWN_DIRECTION;
				break;
		}
		return l10n::getDefaultPlatformModuleMessageCatalog16().formatMessage(locale, dirkey);
	}

}}}

#ifndef REDSTRAIN_DATA_PLATFORM_L10N_OSERRORL10N_HPP
#define REDSTRAIN_DATA_PLATFORM_L10N_OSERRORL10N_HPP

#include <redstrain/locale/Locale.hpp>
#include <redstrain/text/types.hpp>
#include <redstrain/platform/platform.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {
namespace l10n {

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
	typedef int GeneralizedErrorCode;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
	typedef DWORD GeneralizedErrorCode;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

	REDSTRAIN_PLATFORM_L10N_API text::String16 localizeOSErrorMessage16(GeneralizedErrorCode, const locale::Locale&);
	REDSTRAIN_PLATFORM_L10N_API text::String16 localizeOSNetworkingErrorMessage16(GeneralizedErrorCode,
			const locale::Locale&);

}}}

#endif /* REDSTRAIN_DATA_PLATFORM_L10N_OSERRORL10N_HPP */

#ifndef REDSTRAIN_DATA_PLATFORM_L10N_FILEERROR_HPP
#define REDSTRAIN_DATA_PLATFORM_L10N_FILEERROR_HPP

#include <redstrain/locale/Locale.hpp>
#include <redstrain/platform/File.hpp>
#include <redstrain/text/types.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {
namespace l10n {

	REDSTRAIN_PLATFORM_L10N_API text::String16 localizeFileErrorAccessDirection(File::Direction,
			const locale::Locale&);
	REDSTRAIN_PLATFORM_L10N_API text::String16 localizeFileErrorIODirection(File::Direction, const locale::Locale&);

}}}

#endif /* REDSTRAIN_DATA_PLATFORM_L10N_FILEERROR_HPP */

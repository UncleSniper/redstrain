#ifndef REDSTRAIN_DATA_PLATFORM_L10N_SOCKETIOERROR_HPP
#define REDSTRAIN_DATA_PLATFORM_L10N_SOCKETIOERROR_HPP

#include <redstrain/locale/Locale.hpp>
#include <redstrain/platform/SocketIOError.hpp>
#include <redstrain/text/types.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {
namespace l10n {

	REDSTRAIN_PLATFORM_L10N_API text::String16 localizeSocketIOErrorDirection(SocketIOError::Direction,
			const locale::Locale&);

}}}

#endif /* REDSTRAIN_DATA_PLATFORM_L10N_SOCKETIOERROR_HPP */

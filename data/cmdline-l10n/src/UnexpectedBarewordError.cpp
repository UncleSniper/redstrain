#include <redstrain/text/Transcode.hpp>
#include <redstrain/cmdline/UnexpectedBarewordError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace cmdline {

	REDSTRAIN_LOCALIZE_ERROR16(UnexpectedBarewordError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultCommandLineModuleMessageCatalog16().formatMessage(locale,
				l10n::MSG_UNEXPECTED_BAREWORD, Transcode::utf8ToBMP(error.getWord()));
	}

}}

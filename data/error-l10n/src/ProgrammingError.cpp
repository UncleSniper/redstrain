#include <redstrain/text/Transcode.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace error {

	REDSTRAIN_LOCALIZE_ERROR16(ProgrammingError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultErrorModuleMessageCatalog16().formatMessage(locale, l10n::MSG_PROGRAMMING_ERROR,
				Transcode::utf8ToBMP(error.getMessage()));
	}

}}

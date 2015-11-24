#include <redstrain/text/Transcode.hpp>
#include <redstrain/io/MissingInputSeparatorError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace io {

	REDSTRAIN_LOCALIZE_ERROR16(MissingInputSeparatorError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		out << l10n::getDefaultIOModuleMessageCatalog16().formatMessage(
			locale, l10n::MSG_MISSING_INPUT_SEPARATOR_ERROR,
			Transcode::utf8ToBMP(error.getSeparator()),
			Transcode::utf8ToBMP(error.getFile()),
			static_cast<uint32_t>(error.getLineNumber())
		);
	}

}}

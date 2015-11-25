#include <redstrain/text/Transcode.hpp>
#include <redstrain/io/NumberFormatError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Transcode;

namespace redengine {
namespace io {

	REDSTRAIN_LOCALIZE_ERROR16(NumberFormatError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		l10n::IOModuleMessageCatalog16& catalog = l10n::getDefaultIOModuleMessageCatalog16();
		l10n::IOModuleMessageKey typekey;
		switch(error.getRequiredNumberType()) {
			#define clamp(constant) \
				case NumberFormatError::constant: \
					typekey = l10n::MSG_NUMBER_FORMAT_ERROR_ ## constant; \
					break;
			clamp(SIGNED_INTEGER)
			clamp(UNSIGNED_INTEGER)
			#undef clamp
			default:
				typekey = l10n::MSG_NUMBER_FORMAT_ERROR_UNKNOWN_TYPE;
				break;
		}
		out << catalog.formatMessage(
			locale, l10n::MSG_NUMBER_FORMAT_ERROR,
			catalog.formatMessage(locale, typekey),
			Transcode::utf8ToBMP(error.getOffendingRendition())
		);
	}

}}

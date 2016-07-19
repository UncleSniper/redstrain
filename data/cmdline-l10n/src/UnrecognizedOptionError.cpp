#include <redstrain/text/Transcode.hpp>
#include <redstrain/cmdline/UnrecognizedOptionError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Char16;
using redengine::text::String16;
using redengine::text::Transcode;

namespace redengine {
namespace cmdline {

	REDSTRAIN_LOCALIZE_ERROR16(UnrecognizedOptionError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		Char16 initiatorSymbol = static_cast<Char16>(error.getInitiatorSymbol());
		String16 option;
		option += initiatorSymbol;
		if(error.getOptionType() == OptionError::DOUBLE_OPTION)
			option += initiatorSymbol;
		option += Transcode::utf8ToBMP(error.getOptionName());
		out << l10n::getDefaultCommandLineModuleMessageCatalog16().formatMessage(locale,
				l10n::MSG_UNRECOGNIZED_OPTION, option);
	}

}}

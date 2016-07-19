#include <redstrain/text/Transcode.hpp>
#include <redstrain/cmdline/AmbiguousOptionError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::text::Transcode;

namespace redengine {
namespace cmdline {

	REDSTRAIN_LOCALIZE_ERROR16(AmbiguousOptionError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		Char16 initiatorSymbol = static_cast<Char16>(error.getInitiatorSymbol());
		String16 shortOption, longOption;
		shortOption += initiatorSymbol;
		shortOption += static_cast<Char16>(error.getOptionName()[static_cast<string::size_type>(0u)]);
		longOption += initiatorSymbol;
		longOption += initiatorSymbol;
		longOption += Transcode::utf8ToBMP(error.getOptionName());
		out << l10n::getDefaultCommandLineModuleMessageCatalog16().formatMessage(locale,
				l10n::MSG_AMBIGUOUS_OPTION, shortOption, longOption);
	}

}}

#include <redstrain/io/StringOutputStream.hpp>
#include <redstrain/error/ErrorHandlingError.hpp>
#include <redstrain/text/agnostic.hpp>

#include "catalog.hpp"

using redengine::text::Char16;
using redengine::text::String16;
using redengine::text::StreamSink16;
using redengine::io::StringOutputStream;

namespace redengine {
namespace error {

	REDSTRAIN_LOCALIZE_ERROR16(ErrorHandlingError) {
		REDSTRAIN_MAKE_L10N_SINK16;
		const Error* subsequentError = error.getSubsequentError();
		String16 semsg;
		if(subsequentError) {
			StringOutputStream<Char16> sos(semsg);
			StreamSink16 sink(sos);
			subsequentError->printMessage(locale, sink);
		}
		out << l10n::getDefaultErrorModuleMessageCatalog16().formatMessage(locale,
				l10n::MSG_ERROR_HANDLING_ERROR, semsg);
	}

}}

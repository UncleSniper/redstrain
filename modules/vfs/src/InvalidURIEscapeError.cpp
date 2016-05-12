#include <redstrain/text/Transcode.hpp>

#include "InvalidURIEscapeError.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::util::MemorySize;

namespace redengine {
namespace vfs {

	InvalidURIEscapeError::InvalidURIEscapeError(const string& uri, MemorySize escapeOffset)
			: URIEscapeError(uri, escapeOffset) {}

	InvalidURIEscapeError::InvalidURIEscapeError(const String16& uri, MemorySize escapeOffset)
			: URIEscapeError(uri, escapeOffset) {}

	InvalidURIEscapeError::InvalidURIEscapeError(const String32& uri, MemorySize escapeOffset)
			: URIEscapeError(uri, escapeOffset) {}

	InvalidURIEscapeError::InvalidURIEscapeError(const InvalidURIEscapeError& error)
			: Error(error), URIEscapeError(error) {}

	REDSTRAIN_DEFINE_ERROR(InvalidURIEscapeError) {
		out << "Invalid escape sequence in URI component, starting at character " << invalidPartOffset32
				<< " of '" << Transcode::bmpToUTF8(getURI()) << '\'';
	}

}}

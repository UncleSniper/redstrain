#include <redstrain/text/Transcode.hpp>

#include "IncompleteURIEscapeError.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::util::MemorySize;

namespace redengine {
namespace vfs {

	IncompleteURIEscapeError::IncompleteURIEscapeError(const string& uri, MemorySize escapeOffset)
			: URIEscapeError(uri, escapeOffset) {}

	IncompleteURIEscapeError::IncompleteURIEscapeError(const String16& uri, MemorySize escapeOffset)
			: URIEscapeError(uri, escapeOffset) {}

	IncompleteURIEscapeError::IncompleteURIEscapeError(const String32& uri, MemorySize escapeOffset)
			: URIEscapeError(uri, escapeOffset) {}

	IncompleteURIEscapeError::IncompleteURIEscapeError(const IncompleteURIEscapeError& error)
			: Error(error), URIEscapeError(error) {}

	REDSTRAIN_DEFINE_ERROR(IncompleteURIEscapeError) {
		out << "Incomplete escape sequence in URI component, starting at character " << invalidPartOffset32
				<< " of '" << Transcode::bmpToUTF8(getURI()) << '\'';
	}

}}

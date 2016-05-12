#include "URIEscapeError.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::String32;
using redengine::util::MemorySize;

namespace redengine {
namespace vfs {

	URIEscapeError::URIEscapeError(const string& uri, MemorySize escapeOffset)
			: InvalidURIPartError(uri, escapeOffset) {}

	URIEscapeError::URIEscapeError(const String16& uri, MemorySize escapeOffset)
			: InvalidURIPartError(uri, escapeOffset) {}

	URIEscapeError::URIEscapeError(const String32& uri, MemorySize escapeOffset)
			: InvalidURIPartError(uri, escapeOffset) {}

	URIEscapeError::URIEscapeError(const URIEscapeError& error)
			: Error(error), InvalidURIPartError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(URIEscapeError)

}}

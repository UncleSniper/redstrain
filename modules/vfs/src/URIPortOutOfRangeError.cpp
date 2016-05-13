#include <redstrain/text/Transcode.hpp>

#include "URIPortOutOfRangeError.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	URIPortOutOfRangeError::URIPortOutOfRangeError(const string& portRendition)
			: InvalidURIPortError(portRendition) {}

	URIPortOutOfRangeError::URIPortOutOfRangeError(const String16& portRendition)
			: InvalidURIPortError(portRendition) {}

	URIPortOutOfRangeError::URIPortOutOfRangeError(const String32& portRendition)
			: InvalidURIPortError(portRendition) {}

	URIPortOutOfRangeError::URIPortOutOfRangeError(const URIPortOutOfRangeError& error)
			: Error(error), InvalidURIPortError(error) {}

	REDSTRAIN_DEFINE_ERROR(URIPortOutOfRangeError) {
		out << "Port number in URI is outside of legal range: " << Transcode::bmpToUTF8(getPortRendition());
	}

}}

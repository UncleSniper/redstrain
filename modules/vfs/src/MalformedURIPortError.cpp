#include <redstrain/text/Transcode.hpp>

#include "MalformedURIPortError.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	MalformedURIPortError::MalformedURIPortError(const string& portRendition)
			: InvalidURIPortError(portRendition) {}

	MalformedURIPortError::MalformedURIPortError(const String16& portRendition)
			: InvalidURIPortError(portRendition) {}

	MalformedURIPortError::MalformedURIPortError(const String32& portRendition)
			: InvalidURIPortError(portRendition) {}

	MalformedURIPortError::MalformedURIPortError(const MalformedURIPortError& error)
			: Error(error), InvalidURIPortError(error) {}

	REDSTRAIN_DEFINE_ERROR(MalformedURIPortError) {
		out << "Port number in URI is not a number: " << Transcode::bmpToUTF8(getPortRendition());
	}

}}

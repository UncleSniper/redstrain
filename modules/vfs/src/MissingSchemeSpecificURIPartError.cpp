#include <redstrain/text/Transcode.hpp>

#include "MissingSchemeSpecificURIPartError.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	MissingSchemeSpecificURIPartError::MissingSchemeSpecificURIPartError(const string& uri)
			: InvalidURIError(uri) {}

	MissingSchemeSpecificURIPartError::MissingSchemeSpecificURIPartError(const String16& uri)
			: InvalidURIError(uri) {}

	MissingSchemeSpecificURIPartError::MissingSchemeSpecificURIPartError(const String32& uri)
			: InvalidURIError(uri) {}

	MissingSchemeSpecificURIPartError::MissingSchemeSpecificURIPartError(const
			MissingSchemeSpecificURIPartError& error) : Error(error), InvalidURIError(error) {}

	REDSTRAIN_DEFINE_ERROR(MissingSchemeSpecificURIPartError) {
		out << "URI is missing a scheme-specific part: " << Transcode::bmpToUTF8(getURI());
	}

}}

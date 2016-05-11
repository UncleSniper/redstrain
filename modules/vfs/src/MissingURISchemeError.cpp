#include <redstrain/text/Transcode.hpp>

#include "MissingURISchemeError.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	MissingURISchemeError::MissingURISchemeError(const string& uri) : InvalidURIError(uri) {}

	MissingURISchemeError::MissingURISchemeError(const String16& uri) : InvalidURIError(uri) {}

	MissingURISchemeError::MissingURISchemeError(const String32& uri) : InvalidURIError(uri) {}

	MissingURISchemeError::MissingURISchemeError(const MissingURISchemeError& error)
			: Error(error), InvalidURIError(error) {}

	REDSTRAIN_DEFINE_ERROR(MissingURISchemeError) {
		out << "URI has no ':' delimiter separating the scheme from the scheme-specific part: "
				<< Transcode::bmpToUTF8(getURI());
	}

}}

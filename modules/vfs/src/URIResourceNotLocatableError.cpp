#include <redstrain/text/Transcode.hpp>

#include "URIResourceNotLocatableError.hpp"

using redengine::text::String16;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	URIResourceNotLocatableError::URIResourceNotLocatableError(const String16& uri) : URIAcquisitionError(uri) {}

	URIResourceNotLocatableError::URIResourceNotLocatableError(const URIResourceNotLocatableError& error)
			: Error(error), URIAcquisitionError(error) {}

	REDSTRAIN_DEFINE_ERROR(URIResourceNotLocatableError) {
		out << "Cannot acquire referenced resource, URI is not a locator: " << Transcode::bmpToUTF8(getURI());
	}

}}

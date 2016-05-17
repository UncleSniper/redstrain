#include "URIAcquisitionError.hpp"

using redengine::text::String16;

namespace redengine {
namespace vfs {

	URIAcquisitionError::URIAcquisitionError(const String16& uri) : uri(uri) {}

	URIAcquisitionError::URIAcquisitionError(const URIAcquisitionError& error)
			: Error(error), URIError(error), uri(error.uri) {}

	URIAcquisitionError::~URIAcquisitionError() {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(URIAcquisitionError)

}}

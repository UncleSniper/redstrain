#include "URIError.hpp"

namespace redengine {
namespace vfs {

	URIError::URIError() {}

	URIError::URIError(const URIError& error) : Error(error), VFSError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(URIError)

}}

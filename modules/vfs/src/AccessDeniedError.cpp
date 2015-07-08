#include "AccessDeniedError.hpp"

namespace redengine {
namespace vfs {

	AccessDeniedError::AccessDeniedError() {}

	AccessDeniedError::AccessDeniedError(const AccessDeniedError& error) : Error(error), VFSIOError(error) {}

	REDSTRAIN_DEFINE_ERROR(AccessDeniedError) {
		out << "Access permission denied";
	}

}}

#include "AccessDeniedError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	AccessDeniedError::AccessDeniedError(const string& path) : path(path) {}

	AccessDeniedError::AccessDeniedError(const AccessDeniedError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(AccessDeniedError) {
		out << "Access permission denied";
		if(!path.empty())
			out << ": " << path;
	}

}}

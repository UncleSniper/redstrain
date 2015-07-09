#include "IsADirectoryError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	IsADirectoryError::IsADirectoryError(const string& path) : path(path) {}

	IsADirectoryError::IsADirectoryError(const IsADirectoryError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(IsADirectoryError) {
		out << "Is a directory";
		if(!path.empty())
			out << ": " << path;
	}

}}

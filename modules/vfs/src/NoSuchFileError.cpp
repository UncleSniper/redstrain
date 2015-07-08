#include "NoSuchFileError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	NoSuchFileError::NoSuchFileError(const string& path) : path(path) {}

	NoSuchFileError::NoSuchFileError(const NoSuchFileError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(NoSuchFileError) {
		out << "No such file or directory";
		if(!path.empty())
			out << ": " << path;
	}

}}

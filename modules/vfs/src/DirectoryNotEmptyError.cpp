#include "DirectoryNotEmptyError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	DirectoryNotEmptyError::DirectoryNotEmptyError(const string& path) : path(path) {}

	DirectoryNotEmptyError::DirectoryNotEmptyError(const DirectoryNotEmptyError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(DirectoryNotEmptyError) {
		out << "Directory is not empty";
		if(!path.empty())
			out << ": " << path;
	}

}}

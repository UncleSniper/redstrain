#include "FileAlreadyExistsError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	FileAlreadyExistsError::FileAlreadyExistsError(const string& path) : path(path) {}

	FileAlreadyExistsError::FileAlreadyExistsError(const FileAlreadyExistsError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(FileAlreadyExistsError) {
		out << "File already exists";
		if(!path.empty())
			out << ": " << path;
	}

}}

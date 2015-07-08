#include "NotADirectoryError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	NotADirectoryError::NotADirectoryError(const string& path) : path(path) {}

	NotADirectoryError::NotADirectoryError(const NotADirectoryError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(NotADirectoryError) {
		out << "Not a directory";
		if(!path.empty())
			out << ": " << path;
	}

}}

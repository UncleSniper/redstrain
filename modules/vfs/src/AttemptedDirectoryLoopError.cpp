#include "AttemptedDirectoryLoopError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	AttemptedDirectoryLoopError::AttemptedDirectoryLoopError(const string& path) : path(path) {}

	AttemptedDirectoryLoopError::AttemptedDirectoryLoopError(const AttemptedDirectoryLoopError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(AttemptedDirectoryLoopError) {
		out << "Attempted to make ";
		if(path.empty())
			out << "a directory";
		else
			out << '\'' << path << '\'';
		out << " a subdirectory of itsself";
	}

}}

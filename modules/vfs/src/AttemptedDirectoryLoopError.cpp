#include <redstrain/text/Transcode.hpp>

#include "AttemptedDirectoryLoopError.hpp"

using std::string;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	AttemptedDirectoryLoopError::AttemptedDirectoryLoopError(const string& path) : path(path) {}

	AttemptedDirectoryLoopError::AttemptedDirectoryLoopError(const VFS::Pathname& path)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(path, true))) {}

	AttemptedDirectoryLoopError::AttemptedDirectoryLoopError(const VFS::PathIterator& pathBegin,
			const VFS::PathIterator& pathEnd)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true))) {}

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

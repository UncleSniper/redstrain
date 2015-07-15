#include <redstrain/text/Transcode.hpp>

#include "IsADirectoryError.hpp"

using std::string;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	IsADirectoryError::IsADirectoryError(const string& path) : path(path) {}

	IsADirectoryError::IsADirectoryError(const VFS::Pathname& path)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(path, true))) {}

	IsADirectoryError::IsADirectoryError(const VFS::PathIterator& pathBegin, const VFS::PathIterator& pathEnd)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true))) {}

	IsADirectoryError::IsADirectoryError(const IsADirectoryError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(IsADirectoryError) {
		out << "Is a directory";
		if(!path.empty())
			out << ": " << path;
	}

}}

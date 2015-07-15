#include <redstrain/text/Transcode.hpp>

#include "NotADirectoryError.hpp"

using std::string;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	NotADirectoryError::NotADirectoryError(const string& path) : path(path) {}

	NotADirectoryError::NotADirectoryError(const VFS::Pathname& path)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(path, true))) {}

	NotADirectoryError::NotADirectoryError(const VFS::PathIterator& pathBegin, const VFS::PathIterator& pathEnd)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true))) {}

	NotADirectoryError::NotADirectoryError(const NotADirectoryError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(NotADirectoryError) {
		out << "Not a directory";
		if(!path.empty())
			out << ": " << path;
	}

}}

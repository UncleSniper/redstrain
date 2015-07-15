#include <redstrain/text/Transcode.hpp>

#include "NoSuchFileError.hpp"

using std::string;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	NoSuchFileError::NoSuchFileError(const string& path) : path(path) {}

	NoSuchFileError::NoSuchFileError(const VFS::Pathname& path)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(path, true))) {}

	NoSuchFileError::NoSuchFileError(const VFS::PathIterator& pathBegin, const VFS::PathIterator& pathEnd)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true))) {}

	NoSuchFileError::NoSuchFileError(const NoSuchFileError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(NoSuchFileError) {
		out << "No such file or directory";
		if(!path.empty())
			out << ": " << path;
	}

}}

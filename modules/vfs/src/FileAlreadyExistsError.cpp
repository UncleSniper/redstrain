#include <redstrain/text/Transcode.hpp>

#include "FileAlreadyExistsError.hpp"

using std::string;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	FileAlreadyExistsError::FileAlreadyExistsError(const string& path) : path(path) {}

	FileAlreadyExistsError::FileAlreadyExistsError(const VFS::Pathname& path)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(path, true))) {}

	FileAlreadyExistsError::FileAlreadyExistsError(const VFS::PathIterator& pathBegin,
			const VFS::PathIterator& pathEnd)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true))) {}

	FileAlreadyExistsError::FileAlreadyExistsError(const FileAlreadyExistsError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(FileAlreadyExistsError) {
		out << "File already exists";
		if(!path.empty())
			out << ": " << path;
	}

}}

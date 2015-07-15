#include <redstrain/text/Transcode.hpp>

#include "DirectoryNotEmptyError.hpp"

using std::string;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	DirectoryNotEmptyError::DirectoryNotEmptyError(const string& path) : path(path) {}

	DirectoryNotEmptyError::DirectoryNotEmptyError(const VFS::Pathname& path)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(path, true))) {}

	DirectoryNotEmptyError::DirectoryNotEmptyError(const VFS::PathIterator& pathBegin,
			const VFS::PathIterator& pathEnd)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true))) {}

	DirectoryNotEmptyError::DirectoryNotEmptyError(const DirectoryNotEmptyError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(DirectoryNotEmptyError) {
		out << "Directory is not empty";
		if(!path.empty())
			out << ": " << path;
	}

}}

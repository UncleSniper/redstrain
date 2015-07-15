#include <redstrain/text/Transcode.hpp>

#include "TooManySymbolicLinksError.hpp"

using std::string;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	TooManySymbolicLinksError::TooManySymbolicLinksError(const string& path) : path(path) {}

	TooManySymbolicLinksError::TooManySymbolicLinksError(const VFS::Pathname& path)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(path, true))) {}

	TooManySymbolicLinksError::TooManySymbolicLinksError(const VFS::PathIterator& pathBegin,
			const VFS::PathIterator& pathEnd)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true))) {}

	TooManySymbolicLinksError::TooManySymbolicLinksError(const TooManySymbolicLinksError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(TooManySymbolicLinksError) {
		out << "Too many symbolic links";
		if(!path.empty())
			out << ": " << path;
	}

}}

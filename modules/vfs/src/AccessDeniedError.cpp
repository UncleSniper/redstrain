#include <redstrain/text/Transcode.hpp>

#include "AccessDeniedError.hpp"

using std::string;
using redengine::text::Transcode;

namespace redengine {
namespace vfs {

	AccessDeniedError::AccessDeniedError(const string& path) : path(path) {}

	AccessDeniedError::AccessDeniedError(const VFS::Pathname& path)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(path, true))) {}

	AccessDeniedError::AccessDeniedError(const VFS::PathIterator& pathBegin, const VFS::PathIterator& pathEnd)
			: path(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true))) {}

	AccessDeniedError::AccessDeniedError(const AccessDeniedError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(AccessDeniedError) {
		out << "Access permission denied";
		if(!path.empty())
			out << ": " << path;
	}

}}

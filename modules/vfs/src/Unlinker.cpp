#include <redstrain/error/Error.hpp>

#include "Unlinker.hpp"

using redengine::error::Error;
using redengine::text::String16;

namespace redengine {
namespace vfs {

	static VFS::Pathname emptyPath;

	Unlinker::Unlinker(VFS& fs, bool directory)
			: fs(fs), pathBegin(emptyPath.end()), pathEnd(emptyPath.end()), directory(directory) {}

	Unlinker::Unlinker(VFS& fs, const VFS::Pathname& path, bool directory)
			: fs(fs), pathBegin(path.begin()), pathEnd(path.end()), directory(directory) {}

	Unlinker::Unlinker(VFS& fs, VFS::PathIterator pathBegin, VFS::PathIterator pathEnd, bool directory)
			: fs(fs), pathBegin(pathBegin), pathEnd(pathEnd), directory(directory) {}

	Unlinker::Unlinker(const Unlinker& unlinker) : fs(unlinker.fs), pathBegin(unlinker.pathBegin),
			pathEnd(unlinker.pathEnd), directory(unlinker.directory) {}

	Unlinker::~Unlinker() {
		unlink();
	}

	void Unlinker::getPath(VFS::PathIterator& begin, VFS::PathIterator& end) const {
		begin = pathBegin;
		end = pathEnd;
	}

	void Unlinker::unlink() {
		if(pathBegin == pathEnd)
			return;
		try {
			if(directory)
				fs.rmdir(pathBegin, pathEnd);
			else
				fs.unlink(pathBegin, pathEnd);
		}
		catch(const Error&) {}
		catch(...) {}
		pathBegin = pathEnd;
	}

	void Unlinker::release() {
		pathBegin = pathEnd;
	}

	Unlinker& Unlinker::operator=(const VFS::Pathname& path) {
		pathBegin = path.begin();
		pathEnd = path.end();
		return *this;
	}

	Unlinker& Unlinker::operator=(const Unlinker& unlinker) {
		unlink();
		pathBegin = unlinker.pathBegin;
		pathEnd = unlinker.pathEnd;
		return *this;
	}

}}

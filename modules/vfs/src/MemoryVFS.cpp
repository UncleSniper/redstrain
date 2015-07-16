#include <redstrain/platform/ObjectLocker.hpp>

#include "MemoryVFS.hpp"

using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::platform::ObjectLocker;
using redengine::io::BidirectionalStream;

namespace redengine {
namespace vfs {

	// ======== RopeMemoryFile ========

	MemoryVFS::RopeMemoryFile::RopeMemoryFile(MemoryBase& fs, int permissions) : MemoryFile(fs, permissions) {}

	MemoryVFS::RopeMemoryFile::RopeMemoryFile(const RopeMemoryFile& file) : MemoryFile(file) {
		ObjectLocker<MemoryFile> locker(getMemoryBase().getEffectiveMutexPool(), &file);
		content.assign(file.content);
		locker.release();
	}

	Stat::Type MemoryVFS::RopeMemoryFile::getFileType() const {
		return Stat::REGULAR_FILE;
	}

	void MemoryVFS::RopeMemoryFile::stat(Stat& info) {
		info.setSize(content.size());
	}

	void MemoryVFS::RopeMemoryFile::truncate(size_t size) {
		ObjectLocker<MemoryFile> locker(getMemoryBase().getEffectiveMutexPool(), this);
		size_t csz = content.size();
		if(csz > size)
			content.erase(size, csz);
		else if(csz < size)
			content.fill('\0', size - csz);
		locker.release();
	}

	InputStream<char>* MemoryVFS::RopeMemoryFile::getInputStream() {
		return new RopeInputStream(*this);
	}

	OutputStream<char>* MemoryVFS::RopeMemoryFile::getOutputStream() {
		return new RopeOutputStream(*this);
	}

	BidirectionalStream<char>* MemoryVFS::RopeMemoryFile::getStream(bool truncate) {
		return new RopeStream(*this, truncate);
	}

	// ======== RopeBase ========

	// ======== RopeInputStream ========

	// ======== RopeOutputStream ========

	// ======== RopeStream ========

	// ======== MemoryVFS ========

	MemoryVFS::MemoryVFS(MemoryDirectory* root, int baseFlags) : MemoryBase(root, baseFlags) {}

	MemoryVFS::MemoryVFS(int baseFlags) : MemoryBase(NULL, baseFlags) {}

	MemoryVFS::MemoryVFS(const MemoryVFS& vfs) : MemoryBase(vfs) {}

	MemoryBase::MemoryFile* MemoryVFS::createRegularFile(int permissions) {
		return new RopeMemoryFile(*this, permissions);
	}

}}

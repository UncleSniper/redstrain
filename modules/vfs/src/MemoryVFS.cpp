#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/ObjectLocker.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>
#include <redstrain/io/StreamAlreadyClosedError.hpp>
#include <redstrain/io/SeekOffsetOutOfBoundsError.hpp>

#include "MemoryVFS.hpp"

using redengine::vfs::VFS;
using redengine::io::Stream;
using redengine::algorithm::Rope;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::util::StringUtils;
using redengine::platform::ObjectLocker;
using redengine::io::BidirectionalStream;
using redengine::error::IllegalArgumentError;
using redengine::io::StreamAlreadyClosedError;
using redengine::io::SeekOffsetOutOfBoundsError;

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
		requirePermissions(VFS::CAN_WRITE);
		ObjectLocker<MemoryFile> locker(getMemoryBase().getEffectiveMutexPool(), this);
		size_t csz = content.size();
		if(csz > size)
			content.erase(size, csz);
		else if(csz < size)
			content.fill('\0', size - csz);
		locker.release();
	}

	InputStream<char>* MemoryVFS::RopeMemoryFile::getInputStream() {
		requirePermissions(VFS::CAN_READ);
		return new RopeInputStream(*this);
	}

	OutputStream<char>* MemoryVFS::RopeMemoryFile::getOutputStream() {
		requirePermissions(VFS::CAN_WRITE);
		return new RopeOutputStream(*this);
	}

	BidirectionalStream<char>* MemoryVFS::RopeMemoryFile::getStream(bool truncate) {
		requirePermissions(VFS::CAN_READ | VFS::CAN_WRITE);
		return new RopeStream(*this, truncate);
	}

	// ======== RopeBase ========

	MemoryVFS::RopeBase::RopeBase(RopeMemoryFile& file, bool truncate)
			: closed(false), file(file), position(static_cast<size_t>(0u)) {
		file.ref();
		if(truncate) {
			ObjectLocker<MemoryFile> locker(file.getMemoryBase().getEffectiveMutexPool(), &file);
			file.getContent().clear();
			locker.release();
		}
	}

	MemoryVFS::RopeBase::RopeBase(const RopeBase& base)
			: Stream(base), closed(base.closed), file(base.file), position(base.position) {
		file.ref();
	}

	MemoryVFS::RopeBase::~RopeBase() {
		if(!closed) {
			try {
				file.unref();
			}
			catch(...) {}
		}
	}

	void MemoryVFS::RopeBase::close() {
		if(closed)
			return;
		file.unref();
		closed = true;
	}

	void MemoryVFS::RopeBase::seek(off_t offset, SeekWhence whence) {
		if(closed)
			throw StreamAlreadyClosedError();
		size_t fsize = file.getContent().size();
		off_t target;
		switch(whence) {
			case Stream::OFFSET_FROM_START:
				target = offset;
				break;
			case Stream::OFFSET_FROM_HERE:
				target = static_cast<off_t>(position) + offset;
				break;
			case Stream::OFFSET_FROM_END:
				target = static_cast<off_t>(fsize) + offset;
				break;
			default:
				throw IllegalArgumentError("Not a valid SeekWhence: "
						+ StringUtils::toString(static_cast<int>(whence)));
		}
		if(target < static_cast<off_t>(0) || static_cast<size_t>(target) > fsize)
			throw SeekOffsetOutOfBoundsError(target);
		position = static_cast<size_t>(target);
	}

	size_t MemoryVFS::RopeBase::tell() const {
		return position;
	}

	// ======== RopeInputStream ========

	MemoryVFS::RopeInputStream::RopeInputStream(RopeMemoryFile& file, bool truncate) : RopeBase(file, truncate) {}

	MemoryVFS::RopeInputStream::RopeInputStream(const RopeInputStream& stream)
			: Stream(stream), RopeBase(stream), InputStream<char>(stream) {}

	size_t MemoryVFS::RopeInputStream::readBlock(char* buffer, size_t bufferSize) {
		if(isClosed())
			throw StreamAlreadyClosedError();
		ObjectLocker<MemoryFile> locker(file.getMemoryBase().getEffectiveMutexPool(), &file);
		Rope<char>& content = file.getContent();
		size_t fsize = content.size(), count;
		if(position >= fsize)
			count = static_cast<size_t>(0u);
		else {
			count = fsize - position;
			if(count > bufferSize)
				count = bufferSize;
			content.subseq(position, position + count, buffer);
			position += count;
		}
		locker.release();
		return count;
	}

	void MemoryVFS::RopeInputStream::seek(off_t offset, SeekWhence whence) {
		RopeBase::seek(offset, whence);
		atEnd = false;
	}

	// ======== RopeOutputStream ========

	MemoryVFS::RopeOutputStream::RopeOutputStream(RopeMemoryFile& file, bool truncate) : RopeBase(file, truncate) {}

	MemoryVFS::RopeOutputStream::RopeOutputStream(const RopeOutputStream& stream)
			: Stream(stream), RopeBase(stream), OutputStream<char>(stream) {}

	void MemoryVFS::RopeOutputStream::writeBlock(const char* data, size_t dataSize) {
		if(isClosed())
			throw StreamAlreadyClosedError();
		ObjectLocker<MemoryFile> locker(file.getMemoryBase().getEffectiveMutexPool(), &file);
		Rope<char>& content = file.getContent();
		size_t fsize = content.size();
		if(fsize < position) {
			content.fill('\0', position - fsize);
			fsize = position;
		}
		// file:   ...#######
		// write:        ##
		// write:        ######
		size_t end = position + dataSize;
		if(end > fsize)
			end = fsize;
		content.bsplice(position, end, data, data + dataSize);
		locker.release();
	}

	// ======== RopeStream ========

	MemoryVFS::RopeStream::RopeStream(RopeMemoryFile& file, bool truncate)
			: RopeBase(file, truncate), RopeInputStream(file, truncate), RopeOutputStream(file, truncate) {}

	MemoryVFS::RopeStream::RopeStream(const RopeStream& stream)
			: Stream(stream), RopeBase(stream), InputStream<char>(stream), RopeInputStream(stream),
			OutputStream<char>(stream), RopeOutputStream(stream), BidirectionalStream<char>(stream) {}

	// ======== MemoryVFS ========

	MemoryVFS::MemoryVFS(MemoryDirectory* root, int baseFlags) : MemoryBase(root, baseFlags) {}

	MemoryVFS::MemoryVFS(int baseFlags) : MemoryBase(NULL, baseFlags) {}

	MemoryVFS::MemoryVFS(const MemoryVFS& vfs) : MemoryBase(vfs) {}

	MemoryBase::MemoryFile* MemoryVFS::createRegularFile(int permissions) {
		return new RopeMemoryFile(*this, permissions);
	}

}}

#include <new>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/platform/ObjectLocker.hpp>
#include <redstrain/io/IllegalSeekWhenceError.hpp>
#include <redstrain/io/StreamAlreadyClosedError.hpp>
#include <redstrain/io/SeekOffsetOutOfBoundsError.hpp>

#include "MemoryVFS.hpp"

using std::bad_alloc;
using redengine::vfs::VFS;
using redengine::io::Stream;
using redengine::util::FileSize;
using redengine::algorithm::Rope;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::util::FileOffset;
using redengine::util::MemorySize;
using redengine::util::StringUtils;
using redengine::util::IntegerBounds;
using redengine::platform::ObjectLocker;
using redengine::io::BidirectionalStream;
using redengine::io::IllegalSeekWhenceError;
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

	void MemoryVFS::RopeMemoryFile::truncate(FileSize size) {
		requirePermissions(VFS::CAN_WRITE);
		if(size > IntegerBounds<MemorySize>::MAX)
			throw bad_alloc();
		MemorySize rsz = static_cast<MemorySize>(size);
		ObjectLocker<MemoryFile> locker(getMemoryBase().getEffectiveMutexPool(), this);
		MemorySize csz = content.size();
		if(csz > rsz)
			content.erase(rsz, csz);
		else if(csz < rsz)
			content.fill('\0', rsz - csz);
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
			: closed(false), file(file), position(static_cast<MemorySize>(0u)) {
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

	void MemoryVFS::RopeBase::seek(FileOffset offset, SeekWhence whence) {
		if(closed)
			throw StreamAlreadyClosedError();
		MemorySize fsize = file.getContent().size();
		FileOffset target;
		switch(whence) {
			case Stream::OFFSET_FROM_START:
				target = offset;
				break;
			case Stream::OFFSET_FROM_HERE:
				target = static_cast<FileOffset>(position) + offset;
				break;
			case Stream::OFFSET_FROM_END:
				target = static_cast<FileOffset>(fsize) + offset;
				break;
			default:
				throw IllegalSeekWhenceError(whence);
		}
		if(target < static_cast<FileOffset>(0) || static_cast<FileSize>(target) > static_cast<FileSize>(fsize))
			throw SeekOffsetOutOfBoundsError(target);
		position = static_cast<MemorySize>(target);
	}

	FileSize MemoryVFS::RopeBase::tell() const {
		return static_cast<FileSize>(position);
	}

	// ======== RopeInputStream ========

	MemoryVFS::RopeInputStream::RopeInputStream(RopeMemoryFile& file, bool truncate) : RopeBase(file, truncate) {}

	MemoryVFS::RopeInputStream::RopeInputStream(const RopeInputStream& stream)
			: Stream(stream), RopeBase(stream), InputStream<char>(stream) {}

	MemorySize MemoryVFS::RopeInputStream::readBlock(char* buffer, MemorySize bufferSize) {
		if(isClosed())
			throw StreamAlreadyClosedError();
		ObjectLocker<MemoryFile> locker(file.getMemoryBase().getEffectiveMutexPool(), &file);
		Rope<char>& content = file.getContent();
		MemorySize fsize = content.size(), count;
		if(position >= fsize)
			count = static_cast<MemorySize>(0u);
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

	void MemoryVFS::RopeInputStream::seek(FileOffset offset, SeekWhence whence) {
		RopeBase::seek(offset, whence);
		atEnd = false;
	}

	// ======== RopeOutputStream ========

	MemoryVFS::RopeOutputStream::RopeOutputStream(RopeMemoryFile& file, bool truncate) : RopeBase(file, truncate) {}

	MemoryVFS::RopeOutputStream::RopeOutputStream(const RopeOutputStream& stream)
			: Stream(stream), RopeBase(stream), OutputStream<char>(stream) {}

	void MemoryVFS::RopeOutputStream::writeBlock(const char* data, MemorySize dataSize) {
		if(isClosed())
			throw StreamAlreadyClosedError();
		ObjectLocker<MemoryFile> locker(file.getMemoryBase().getEffectiveMutexPool(), &file);
		Rope<char>& content = file.getContent();
		MemorySize fsize = content.size();
		if(fsize < position) {
			content.fill('\0', position - fsize);
			fsize = position;
		}
		// file:   ...#######
		// write:        ##
		// write:        ######
		MemorySize end = position + dataSize;
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

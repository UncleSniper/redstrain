#include <ctime>
#include <redstrain/util/Ref.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/platform/ObjectLocker.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "MemoryVFSBase.hpp"
#include "NoSuchFileError.hpp"
#include "AccessDeniedError.hpp"
#include "IsADirectoryError.hpp"
#include "NotADirectoryError.hpp"
#include "NotASymbolicLinkError.hpp"

using redengine::util::Ref;
using redengine::text::String16;
using redengine::util::Appender;
using redengine::text::Transcode;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::platform::ObjectLocker;
using redengine::io::BidirectionalStream;
using redengine::error::ProgrammingError;

namespace redengine {
namespace vfs {

	MemoryVFSBase::MemoryFile::MemoryFile(MemoryVFSBase& fs, int permissions)
			: fs(fs), owner(fs.getCurrentUser()), group(fs.getCurrentGroup()), permissions(permissions) {
		atime = mtime = ctime = time(NULL);
	}

	MemoryVFSBase::MemoryFile::MemoryFile(const MemoryFile& file) : ReferenceCounted(file),
			fs(file.fs), owner(file.owner), group(file.group), permissions(file.permissions),
			atime(file.atime), mtime(file.mtime), ctime(file.ctime) {}

	MemoryVFSBase::MemoryFile::~MemoryFile() {}

	void MemoryVFSBase::MemoryFile::ref() {
		ObjectLocker<MemoryFile> locker(fs.getEffectiveMutexPool(), this);
		ReferenceCounted::ref();
		locker.release();
	}

	void MemoryVFSBase::MemoryFile::unref() {
		ObjectLocker<MemoryFile> locker(fs.getEffectiveMutexPool(), this);
		ReferenceCounted::unref();
		locker.release();
	}

	void MemoryVFSBase::MemoryFile::requireOwner() const {
		if((fs.flags & (BFL_ENFORCE_PERMISSIONS | BFL_HEED_OWNER)) != (BFL_ENFORCE_PERMISSIONS | BFL_HEED_OWNER))
			return;
		if(fs.currentUser != owner)
			throw AccessDeniedError();
	}

	void MemoryVFSBase::MemoryFile::requirePermissions(int mask) {
		if(!access(mask))
			throw AccessDeniedError();
	}

	void MemoryVFSBase::MemoryFile::chmod(int permissions) {
		requireOwner();
		this->permissions = permissions & 0777;
	}

	void MemoryVFSBase::MemoryFile::chown(Stat::UserID owner) {
		requireOwner();
		this->owner = owner;
	}

	void MemoryVFSBase::MemoryFile::chgrp(Stat::GroupID group) {
		requireOwner();
		this->group = group;
	}

	void MemoryVFSBase::MemoryFile::utime() {
		requirePermissions(2);
		time_t now = time(NULL);
		atime = mtime = ctime = now;
	}

	void MemoryVFSBase::MemoryFile::utime(time_t accessTimestamp, time_t modificationTimestamp) {
		requirePermissions(2);
		atime = accessTimestamp;
		mtime = modificationTimestamp;
		ctime = time(NULL);
	}

	bool MemoryVFSBase::MemoryFile::access(int mask) {
		if(!(fs.flags & BFL_ENFORCE_PERMISSIONS))
			return true;
		unsigned shift;
		if(!(fs.flags & BFL_HEED_OWNER) || fs.currentUser == owner)
			shift = 6u;
		else if(!(fs.flags & BFL_HEED_GROUP) || fs.currentGroup == group)
			shift = 3u;
		else
			shift = 0u;
		return ((permissions >> shift) & 3) == (mask & 3);
	}

	void MemoryVFSBase::MemoryFile::rmdir() {
		throw NotADirectoryError();
	}

	void MemoryVFSBase::MemoryFile::readlink(String16&) {
		throw NotASymbolicLinkError();
	}

	MemoryVFSBase::MemoryDirectory::MemoryDirectory(MemoryVFSBase& fs, int permissions)
			: MemoryFile(fs, permissions) {}

	MemoryVFSBase::MemoryDirectory::MemoryDirectory(const MemoryDirectory& file) : MemoryFile(file) {}

	void MemoryVFSBase::MemoryDirectory::removeEntry(const String16& name) {
		putEntry(name, NULL);
	}

	Stat::Type MemoryVFSBase::MemoryDirectory::getFileType() const {
		return Stat::DIRECTORY;
	}

	void MemoryVFSBase::MemoryDirectory::truncate(size_t) {
		throw IsADirectoryError();
	}

	InputStream<char>* MemoryVFSBase::MemoryDirectory::getInputStream() {
		throw IsADirectoryError();
	}

	OutputStream<char>* MemoryVFSBase::MemoryDirectory::getOutputStream() {
		throw IsADirectoryError();
	}

	BidirectionalStream<char>* MemoryVFSBase::MemoryDirectory::getStream(bool) {
		throw IsADirectoryError();
	}

	MemoryVFSBase::SimpleMemoryDirectory::SimpleMemoryDirectory(MemoryVFSBase& fs, int permissions)
			: MemoryDirectory(fs, permissions) {}

	MemoryVFSBase::SimpleMemoryDirectory::SimpleMemoryDirectory(const SimpleMemoryDirectory& file)
			: MemoryDirectory(file) {
		file.copydir(*this);
	}

	MemoryVFSBase::SimpleMemoryDirectory::~SimpleMemoryDirectory() {
		ConstEntryIterator begin(entries.begin()), end(entries.end());
		for(; begin != end; ++begin)
			begin->second->unref();
	}

	void MemoryVFSBase::SimpleMemoryDirectory::stat(Stat& info) {
		ObjectLocker<MemoryFile> locker(getMemoryVFSBase().getEffectiveMutexPool(), this);
		info.setSize(static_cast<size_t>(entries.size()));
		locker.release();
	}

	void MemoryVFSBase::SimpleMemoryDirectory::readdir(Appender<String16>& sink) {
		ObjectLocker<MemoryFile> locker(getMemoryVFSBase().getEffectiveMutexPool(), this);
		ConstEntryIterator begin(entries.begin()), end(entries.end());
		for(; begin != end; ++begin)
			sink.append(begin->first);
		sink.doneAppending();
	}

	MemoryVFSBase::MemoryFile* MemoryVFSBase::SimpleMemoryDirectory::getEntry(const String16& name) {
		ObjectLocker<MemoryFile> locker(getMemoryVFSBase().getEffectiveMutexPool(), this);
		ConstEntryIterator it(entries.find(name));
		Ref<MemoryFile> file;
		if(it != entries.end())
			file.move(it->second);
		locker.release();
		return file.set();
	}

	void MemoryVFSBase::SimpleMemoryDirectory::putEntry(const String16& name, MemoryFile* file) {
		ObjectLocker<MemoryFile> locker(getMemoryVFSBase().getEffectiveMutexPool(), this);
		EntryIterator it(entries.find(name));
		if(it != entries.end()) {
			it->second->unref();
			entries.erase(it);
		}
		if(file)
			entries[name] = file;
		locker.release();
	}

	MemoryVFSBase::MemoryVFSBase(MemoryDirectory* root, int flags) : root(root), mutexPool(NULL),
			currentUser(Stat::NO_USER), currentGroup(Stat::NO_GROUP), flags(flags) {
		root->ref();
	}

	MemoryVFSBase::MemoryVFSBase(const MemoryVFSBase& base)
			: VFS(base), root(base.root), mutexPool(base.mutexPool),
			currentUser(base.currentUser), currentGroup(base.currentGroup), flags(base.flags) {
		root->ref();
	}

	MemoryVFSBase::~MemoryVFSBase() {
		root->unref();
	}

	MemoryVFSBase::MemoryFile* MemoryVFSBase::resolvePath(PathIterator& begin, PathIterator end) const {
		PathIterator oldBegin(begin);
		Ref<MemoryFile> file(root, true);
		for(; begin != end; ++begin) {
			if(file->getFileType() != Stat::DIRECTORY)
				throw NotADirectoryError(Transcode::bmpToUTF8(VFS::constructPathname(oldBegin, begin, true)));
			MemoryFile* child = static_cast<MemoryDirectory*>(*file)->getEntry(*begin);
			if(!child) {
				file.move();
				return NULL;
			}
			file->unref();
			file = child;
		}
		return file.set();
	}

	MemoryVFSBase::MemoryFile* MemoryVFSBase::requireFile(PathIterator pathBegin, PathIterator pathEnd) const {
		PathIterator center(pathBegin);
		MemoryFile* file = resolvePath(center, pathEnd);
		if(file)
			return file;
		throw NoSuchFileError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, ++center, true)));
	}

	MemoryVFSBase::MemoryFile* MemoryVFSBase::requireParentDirectory(PathIterator pathBegin,
			PathIterator pathEnd) const {
		PathIterator oldBegin(pathBegin), nextSegment;
		Ref<MemoryFile> file(root, true);
		for(; pathBegin != pathEnd; pathBegin = nextSegment) {
			nextSegment = pathBegin;
			if(++nextSegment == pathEnd)
				return file.set();
			if(file->getFileType() != Stat::DIRECTORY)
				throw NotADirectoryError(Transcode::bmpToUTF8(VFS::constructPathname(oldBegin, pathBegin, true)));
			MemoryFile* child = static_cast<MemoryDirectory*>(*file)->getEntry(*pathBegin);
			if(!child) {
				file.move();
				return NULL;
			}
			file->unref();
			file = child;
		}
		throw ProgrammingError("MemoryVFSBase::requireParentDirectory() loop should never terminate");
	}

	MemoryVFSBase::MemoryFile* MemoryVFSBase::snapSymbolicLinks(PathIterator pathBegin, PathIterator pathEnd,
			MemoryFile* file) const {
		Ref<MemoryFile> current(file);
		Pathname stack;
		stack.insert(stack.begin(), pathBegin, pathEnd);
		while(current->getFileType() == Stat::SYMBOLIC_LINK) {
			String16 refpath;
			current->readlink(refpath);
			VFS::deconstructPathname(refpath, stack);
			if(stack.empty())
				stack.push_back(VFS::PARENT_LINK);
			else
				stack.pop_back();
			MemoryFile* child = requireFile(stack.begin(), stack.end());
			child->ref();
			current.move();
			current = child;
		}
		return current.set();
	}

	void MemoryVFSBase::stat(PathIterator pathBegin, PathIterator pathEnd, Stat& info, bool ofLink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		if(!ofLink)
			file = MemoryVFSBase::snapSymbolicLinks(pathBegin, pathEnd, *file);
		info.setType(file->getFileType());
		info.setOwner(file->getOwner());
		info.setGroup(file->getGroup());
		info.setDevice(Stat::NO_DEVICE);
		info.setPermissions(file->getPermissions());
		info.setSize(static_cast<size_t>(0u));
		info.setAccessTimestamp(file->getAccessTimestamp());
		info.setModificationTimestamp(file->getModificationTimestamp());
		info.setStatusChangeTimestamp(file->getStatusChangeTimestamp());
		file->stat(info);
		file.move();
	}

	void MemoryVFSBase::chmod(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		file->chmod(permissions);
		file.move();
	}

	void MemoryVFSBase::chown(PathIterator pathBegin, PathIterator pathEnd, Stat::UserID owner, bool ofLink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		if(!ofLink)
			file = MemoryVFSBase::snapSymbolicLinks(pathBegin, pathEnd, *file);
		file->chown(owner);
		file.move();
	}

	void MemoryVFSBase::chgrp(PathIterator pathBegin, PathIterator pathEnd, Stat::GroupID group, bool ofLink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		if(!ofLink)
			file = MemoryVFSBase::snapSymbolicLinks(pathBegin, pathEnd, *file);
		file->chgrp(group);
		file.move();
	}

	/*
	void MemoryVFSBase::link(PathIterator, PathIterator, PathIterator, PathIterator);
	void MemoryVFSBase::unlink(PathIterator, PathIterator);
	void MemoryVFSBase::utime(PathIterator, PathIterator);
	void MemoryVFSBase::utime(PathIterator, PathIterator, time_t, time_t);
	bool MemoryVFSBase::access(PathIterator, PathIterator, int);
	void MemoryVFSBase::sync();
	void MemoryVFSBase::rename(PathIterator, PathIterator, PathIterator, PathIterator);
	void MemoryVFSBase::mkdir(PathIterator, PathIterator, int);
	void MemoryVFSBase::rmdir(PathIterator, PathIterator);
	void MemoryVFSBase::symlink(PathIterator, PathIterator, const String16&);
	void MemoryVFSBase::readlink(PathIterator, PathIterator, String16&);
	void MemoryVFSBase::readdir(PathIterator, PathIterator, Appender<String16>&);
	void MemoryVFSBase::truncate(PathIterator, PathIterator, size_t);
	void MemoryVFSBase::statfs(PathIterator, PathIterator, FSInfo&);
	void MemoryVFSBase::mknod(PathIterator, PathIterator, Stat::Type, int, Stat::DeviceID);
	InputStream<char>* MemoryVFSBase::getInputStream(PathIterator, PathIterator);
	OutputStream<char>* MemoryVFSBase::getOutputStream(PathIterator, PathIterator);
	BidirectionalStream<char>* MemoryVFSBase::getStream(PathIterator, PathIterator, bool);
	VFile* MemoryVFSBase::getFileReference(PathIterator, PathIterator);
	*/

}}

#include <ctime>
#include <redstrain/util/Ref.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/platform/ObjectLocker.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "MemoryBase.hpp"
#include "NoSuchFileError.hpp"
#include "AccessDeniedError.hpp"
#include "IsADirectoryError.hpp"
#include "NotADirectoryError.hpp"
#include "NotASymbolicLinkError.hpp"
#include "FileAlreadyExistsError.hpp"
#include "DirectoryNotEmptyError.hpp"
#include "UnsupportedFileTypeError.hpp"
#include "TooManySymbolicLinksError.hpp"
#include "AttemptedDirectoryLoopError.hpp"
#include "UnsupportedDeviceFileOperationError.hpp"
#include "tweaks.hpp"

using std::list;
using redengine::util::Ref;
using redengine::text::String16;
using redengine::util::Appender;
using redengine::text::Transcode;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::util::IntegerBounds;
using redengine::platform::ObjectLocker;
using redengine::io::BidirectionalStream;
using redengine::error::ProgrammingError;

namespace redengine {
namespace vfs {

	// ======== MemoryFile ========

	MemoryBase::MemoryFile::MemoryFile(MemoryBase& fs, int permissions)
			: fs(fs), owner(fs.getCurrentUser()), group(fs.getCurrentGroup()), permissions(permissions) {
		atime = mtime = ctime = time(NULL);
	}

	MemoryBase::MemoryFile::MemoryFile(const MemoryFile& file) : ReferenceCounted(file),
			fs(file.fs), owner(file.owner), group(file.group), permissions(file.permissions),
			atime(file.atime), mtime(file.mtime), ctime(file.ctime) {}

	MemoryBase::MemoryFile::~MemoryFile() {}

	void MemoryBase::MemoryFile::ref() {
		ObjectLocker<MemoryFile> locker(fs.getEffectiveMutexPool(), this);
		ReferenceCounted::ref();
		locker.release();
	}

	void MemoryBase::MemoryFile::unref() {
		ObjectLocker<MemoryFile> locker(fs.getEffectiveMutexPool(), this);
		ReferenceCounted::unref();
		locker.release();
	}

	void MemoryBase::MemoryFile::requireOwner() const {
		if((fs.flags & (BFL_ENFORCE_PERMISSIONS | BFL_HEED_OWNER)) != (BFL_ENFORCE_PERMISSIONS | BFL_HEED_OWNER))
			return;
		if(fs.currentUser != owner)
			throw AccessDeniedError();
	}

	void MemoryBase::MemoryFile::requirePermissions(int mask) {
		if(!access(mask))
			throw AccessDeniedError();
	}

	void MemoryBase::MemoryFile::chmod(int permissions) {
		requireOwner();
		this->permissions = permissions & 0777;
	}

	void MemoryBase::MemoryFile::chown(Stat::UserID owner) {
		requireOwner();
		this->owner = owner;
	}

	void MemoryBase::MemoryFile::chgrp(Stat::GroupID group) {
		requireOwner();
		this->group = group;
	}

	void MemoryBase::MemoryFile::utime() {
		requirePermissions(2);
		time_t now = time(NULL);
		atime = mtime = ctime = now;
	}

	void MemoryBase::MemoryFile::utime(time_t accessTimestamp, time_t modificationTimestamp) {
		requirePermissions(2);
		atime = accessTimestamp;
		mtime = modificationTimestamp;
		ctime = time(NULL);
	}

	bool MemoryBase::MemoryFile::access(int mask) {
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

	void MemoryBase::MemoryFile::readlink(String16&) {
		throw NotASymbolicLinkError();
	}

	// ======== MemoryDirectory ========

	MemoryBase::MemoryDirectory::MemoryDirectory(MemoryBase& fs, int permissions)
			: MemoryFile(fs, permissions) {}

	MemoryBase::MemoryDirectory::MemoryDirectory(const MemoryDirectory& file) : MemoryFile(file) {}

	void MemoryBase::MemoryDirectory::removeEntry(const String16& name) {
		putEntry(name, NULL);
	}

	Stat::Type MemoryBase::MemoryDirectory::getFileType() const {
		return Stat::DIRECTORY;
	}

	void MemoryBase::MemoryDirectory::truncate(size_t) {
		throw IsADirectoryError();
	}

	InputStream<char>* MemoryBase::MemoryDirectory::getInputStream() {
		throw IsADirectoryError();
	}

	OutputStream<char>* MemoryBase::MemoryDirectory::getOutputStream() {
		throw IsADirectoryError();
	}

	BidirectionalStream<char>* MemoryBase::MemoryDirectory::getStream(bool) {
		throw IsADirectoryError();
	}

	// ======== SimpleMemoryDirectory ========

	MemoryBase::SimpleMemoryDirectory::SimpleMemoryDirectory(MemoryBase& fs, int permissions)
			: MemoryDirectory(fs, permissions) {}

	MemoryBase::SimpleMemoryDirectory::SimpleMemoryDirectory(const SimpleMemoryDirectory& file)
			: MemoryDirectory(file) {
		file.copydir(*this);
	}

	MemoryBase::SimpleMemoryDirectory::~SimpleMemoryDirectory() {
		ConstEntryIterator begin(entries.begin()), end(entries.end());
		for(; begin != end; ++begin)
			begin->second->unref();
	}

	void MemoryBase::SimpleMemoryDirectory::stat(Stat& info) {
		ObjectLocker<MemoryFile> locker(getMemoryVFSBase().getEffectiveMutexPool(), this);
		info.setSize(static_cast<size_t>(entries.size()));
		locker.release();
	}

	void MemoryBase::SimpleMemoryDirectory::readdir(Appender<String16>& sink) {
		ObjectLocker<MemoryFile> locker(getMemoryVFSBase().getEffectiveMutexPool(), this);
		ConstEntryIterator begin(entries.begin()), end(entries.end());
		for(; begin != end; ++begin)
			sink.append(begin->first);
		locker.release();
		sink.doneAppending();
	}

	void MemoryBase::SimpleMemoryDirectory::copydir(MemoryDirectory& sink) const {
		ObjectLocker<MemoryFile> locker(getMemoryVFSBase().getEffectiveMutexPool(), this);
		ConstEntryIterator begin(entries.begin()), end(entries.end());
		for(; begin != end; ++begin)
			sink.putEntry(begin->first, begin->second);
		locker.release();
	}

	MemoryBase::MemoryFile* MemoryBase::SimpleMemoryDirectory::getEntry(const String16& name) {
		ObjectLocker<MemoryFile> locker(getMemoryVFSBase().getEffectiveMutexPool(), this);
		ConstEntryIterator it(entries.find(name));
		Ref<MemoryFile> file;
		if(it != entries.end())
			file.move(it->second);
		locker.release();
		return file.set();
	}

	void MemoryBase::SimpleMemoryDirectory::putEntry(const String16& name, MemoryFile* file) {
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

	bool MemoryBase::SimpleMemoryDirectory::isEmptyDirectory() const {
		ObjectLocker<MemoryFile> locker(getMemoryVFSBase().getEffectiveMutexPool(), this);
		bool empty = entries.empty();
		locker.release();
		return empty;
	}

	// ======== SimpleMemorySymlink ========

	MemoryBase::SimpleMemorySymlink::SimpleMemorySymlink(MemoryBase& fs, int permissions, const String16& target)
			: MemoryFile(fs, permissions), target(target) {}

	MemoryBase::SimpleMemorySymlink::SimpleMemorySymlink(const SimpleMemorySymlink& link)
			: MemoryFile(link), target(link.target) {}

	Stat::Type MemoryBase::SimpleMemorySymlink::getFileType() const {
		return Stat::SYMBOLIC_LINK;
	}

	void MemoryBase::SimpleMemorySymlink::stat(Stat&) {}

	void MemoryBase::SimpleMemorySymlink::readlink(String16& result) {
		result = target;
	}

	void MemoryBase::SimpleMemorySymlink::truncate(size_t) {
		throw ProgrammingError("MemoryBase should not attempt to call MemoryFile::truncate() on a symlink");
	}

	InputStream<char>* MemoryBase::SimpleMemorySymlink::getInputStream() {
		throw ProgrammingError("MemoryBase should not attempt to call MemoryFile::getInputStream() on a symlink");
	}

	OutputStream<char>* MemoryBase::SimpleMemorySymlink::getOutputStream() {
		throw ProgrammingError("MemoryBase should not attempt to call MemoryFile::getOutputStream() on a symlink");
	}

	BidirectionalStream<char>* MemoryBase::SimpleMemorySymlink::getStream(bool) {
		throw ProgrammingError("MemoryBase should not attempt to call MemoryFile::getStream() on a symlink");
	}

	// ======== SimpleMemoryDeviceFile ========

	MemoryBase::SimpleMemoryDeviceFile::SimpleMemoryDeviceFile(MemoryBase& fs, int permissions,
			bool block, Stat::DeviceID device) : MemoryFile(fs, permissions), block(block), device(device) {}

	MemoryBase::SimpleMemoryDeviceFile::SimpleMemoryDeviceFile(const SimpleMemoryDeviceFile& file)
			: MemoryFile(file), block(file.block), device(file.device) {}

	Stat::Type MemoryBase::SimpleMemoryDeviceFile::getFileType() const {
		return block ? Stat::BLOCK_DEVICE : Stat::CHARACTER_DEVICE;
	}

	void MemoryBase::SimpleMemoryDeviceFile::stat(Stat& info) {
		info.setSpecialSpecifier(device);
	}

	void MemoryBase::SimpleMemoryDeviceFile::truncate(size_t size) {
		getMemoryVFSBase().truncateDevice(block, device, size);
	}

	InputStream<char>* MemoryBase::SimpleMemoryDeviceFile::getInputStream() {
		return getMemoryVFSBase().getDeviceInputStream(block, device);
	}

	OutputStream<char>* MemoryBase::SimpleMemoryDeviceFile::getOutputStream() {
		return getMemoryVFSBase().getDeviceOutputStream(block, device);
	}

	BidirectionalStream<char>* MemoryBase::SimpleMemoryDeviceFile::getStream(bool truncate) {
		return getMemoryVFSBase().getDeviceStream(block, device, truncate);
	}

	// ======== TreePath ========

	struct UnrefPartialTreePath {

		list<MemoryBase::MemoryFile*>* stack;
		size_t count;

		UnrefPartialTreePath(list<MemoryBase::MemoryFile*>* stack) : stack(stack), count(static_cast<size_t>(0u)) {}
		UnrefPartialTreePath(const UnrefPartialTreePath& pointer) : stack(pointer.stack), count(pointer.count) {}

		~UnrefPartialTreePath() {
			if(!stack)
				return;
			list<MemoryBase::MemoryFile*>::const_iterator it(stack->begin());
			for(; count; --count, ++it)
				(*it)->unref();
		}

	};

	struct PopTreePath {

		list<MemoryBase::MemoryFile*>* stack;

		PopTreePath(list<MemoryBase::MemoryFile*>* stack) : stack(stack) {}
		PopTreePath(const PopTreePath& pointer) : stack(pointer.stack) {}

		~PopTreePath() {
			if(stack)
				stack->pop_back();
		}

	};

	MemoryBase::TreePath::TreePath() {}

	MemoryBase::TreePath::TreePath(const TreePath& path) {
		list<MemoryFile*>::const_iterator begin(path.stack.begin()), end(path.stack.end());
		UnrefPartialTreePath unref(&stack);
		for(; begin != end; ++begin) {
			pushFile(**begin);
			++unref.count;
		}
		unref.stack = NULL;
	}

	MemoryBase::TreePath::~TreePath() {
		clear();
	}

	void MemoryBase::TreePath::pushFile(MemoryFile& file) {
		stack.push_back(&file);
		PopTreePath pop(&stack);
		file.ref();
		pop.stack = NULL;
	}

	void MemoryBase::TreePath::clear() {
		list<MemoryFile*>::const_iterator begin(stack.begin()), end(stack.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		stack.clear();
	}

	bool MemoryBase::TreePath::contains(MemoryFile& file) const {
		list<MemoryFile*>::const_iterator begin(stack.begin()), end(stack.end());
		for(; begin != end; ++begin) {
			if(*begin == &file)
				return true;
		}
		return false;
	}

	// ======== MemoryBase ========

	MemoryBase::MemoryBase(MemoryDirectory* root, int flags) : root(root), mutexPool(NULL),
			currentUser(Stat::NO_USER), currentGroup(Stat::NO_GROUP), flags(flags) {
		root->ref();
	}

	MemoryBase::MemoryBase(const MemoryBase& base)
			: VFS(base), root(base.root), mutexPool(base.mutexPool),
			currentUser(base.currentUser), currentGroup(base.currentGroup), flags(base.flags) {
		root->ref();
	}

	MemoryBase::~MemoryBase() {
		root->unref();
	}

	MemoryBase::MemoryFile* MemoryBase::resolvePath(PathIterator& begin, PathIterator end,
			unsigned symlinkDepth, TreePath* trace) const {
		PathIterator oldBegin(begin);
		Pathname actualPath;
		Ref<MemoryFile> file(root, true);
		for(; begin != end; ++begin) {
			file = snapSymbolicLinks(actualPath.begin(), actualPath.end(), file.set(), false,
					&actualPath, symlinkDepth, trace);
			if(file->getFileType() != Stat::DIRECTORY) {
				file.move();
				throw NotADirectoryError(Transcode::bmpToUTF8(VFS::constructPathname(oldBegin, begin, true)));
			}
			if(!file->access(MemoryBase::DIRECTORY_SEARCH_PERMISSIONS)) {
				file.move();
				throw AccessDeniedError(Transcode::bmpToUTF8(VFS::constructPathname(oldBegin, begin, true)));
			}
			MemoryFile* child = static_cast<MemoryDirectory*>(*file)->getEntry(*begin);
			if(!child) {
				file.move();
				return NULL;
			}
			file->unref();
			file = child;
			actualPath.push_back(*begin);
			if(trace)
				trace->pushFile(*child);
		}
		return file.set();
	}

	MemoryBase::MemoryFile* MemoryBase::requireFile(PathIterator pathBegin, PathIterator pathEnd,
			unsigned symlinkDepth, TreePath* trace) const {
		PathIterator center(pathBegin);
		MemoryFile* file = resolvePath(center, pathEnd, symlinkDepth, trace);
		if(file)
			return file;
		throw NoSuchFileError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, ++center, true)));
	}

	MemoryBase::MemoryDirectory* MemoryBase::requireParentDirectory(PathIterator pathBegin,
			PathIterator pathEnd, String16* basename, unsigned symlinkDepth, TreePath* trace) const {
		PathIterator oldBegin(pathBegin), nextSegment;
		Pathname actualPath;
		Ref<MemoryFile> file(root, true);
		if(trace)
			trace->pushFile(*root);
		for(; pathBegin != pathEnd; pathBegin = nextSegment) {
			nextSegment = pathBegin;
			file = snapSymbolicLinks(actualPath.begin(), actualPath.end(), file.set(),
					false, &actualPath, symlinkDepth, trace);
			if(file->getFileType() != Stat::DIRECTORY) {
				file.move();
				throw NotADirectoryError(Transcode::bmpToUTF8(VFS::constructPathname(oldBegin, pathBegin, true)));
			}
			if(!file->access(MemoryBase::DIRECTORY_SEARCH_PERMISSIONS)) {
				file.move();
				throw AccessDeniedError(Transcode::bmpToUTF8(VFS::constructPathname(oldBegin, pathEnd, true)));
			}
			MemoryDirectory* dir = static_cast<MemoryDirectory*>(*file);
			if(++nextSegment == pathEnd) {
				if(basename)
					*basename = *pathBegin;
				file.set();
				return dir;
			}
			MemoryFile* child = dir->getEntry(*pathBegin);
			if(!child) {
				file.move();
				throw NoSuchFileError(Transcode::bmpToUTF8(VFS::constructPathname(oldBegin, nextSegment, true)));
			}
			file->unref();
			file = child;
			actualPath.push_back(*pathBegin);
			if(trace)
				trace->pushFile(*child);
		}
		if(basename)
			basename->clear();
		return static_cast<MemoryDirectory*>(file.set());
	}

	MemoryBase::MemoryFile* MemoryBase::snapSymbolicLinks(PathIterator pathBegin, PathIterator pathEnd,
			MemoryFile* file, bool mayBeMissing, Pathname* newPath, unsigned symlinkDepth, TreePath* trace) const {
		Ref<MemoryFile> current(file);
		Pathname stack;
		stack.insert(stack.begin(), pathBegin, pathEnd);
		while(current->getFileType() == Stat::SYMBOLIC_LINK) {
			if(++symlinkDepth > REDSTRAIN_VFS_MAX_SYMLINK_DEPTH) {
				current.move();
				throw TooManySymbolicLinksError(Transcode::bmpToUTF8(VFS::constructPathname(stack.begin(),
						stack.end(), true)));
			}
			String16 refpath;
			current->readlink(refpath);
			if(stack.empty())
				stack.push_back(VFS::PARENT_LINK);
			else
				stack.pop_back();
			VFS::deconstructPathname(refpath, stack);
			if(trace)
				trace->clear();
			MemoryFile* child;
			if(mayBeMissing) {
				PathIterator center(stack.begin());
				child = resolvePath(center, stack.end(), symlinkDepth, trace);
				if(!child) {
					current.move();
					return NULL;
				}
			}
			else
				child = requireFile(stack.begin(), stack.end(), symlinkDepth, trace);
			current.move();
			current = child;
		}
		if(newPath)
			*newPath = stack;
		return current.set();
	}

	MemoryBase::MemoryDirectory* MemoryBase::createDirectory(int permissions) {
		return new SimpleMemoryDirectory(*this, permissions);
	}

	MemoryBase::MemoryFile* MemoryBase::createSymlink(const String16& target) {
		return new SimpleMemorySymlink(*this, 0777, target);
	}

	MemoryBase::MemoryFile* MemoryBase::createDeviceFile(int permissions, bool block, Stat::DeviceID device) {
		return new SimpleMemoryDeviceFile(*this, permissions, block, device);
	}

	void MemoryBase::truncateDevice(bool block, Stat::DeviceID device, size_t) {
		throw UnsupportedDeviceFileOperationError(block, device, UnsupportedDeviceFileOperationError::TRUNCATE);
	}

	InputStream<char>* MemoryBase::getDeviceInputStream(bool block, Stat::DeviceID device) {
		throw UnsupportedDeviceFileOperationError(block, device,
				UnsupportedDeviceFileOperationError::OPEN_FOR_READING);
	}

	OutputStream<char>* MemoryBase::getDeviceOutputStream(bool block, Stat::DeviceID device) {
		throw UnsupportedDeviceFileOperationError(block, device,
				UnsupportedDeviceFileOperationError::OPEN_FOR_WRITING);
	}

	BidirectionalStream<char>* MemoryBase::getDeviceStream(bool block, Stat::DeviceID device, bool) {
		throw UnsupportedDeviceFileOperationError(block, device,
				UnsupportedDeviceFileOperationError::OPEN_FOR_RANDOM_ACCESS);
	}

	void MemoryBase::stat(PathIterator pathBegin, PathIterator pathEnd, Stat& info, bool ofLink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		if(!ofLink)
			file = snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
		info.setType(file->getFileType());
		info.setOwner(file->getOwner());
		info.setGroup(file->getGroup());
		info.setDevice(Stat::NO_DEVICE);
		info.setSpecialSpecifier(Stat::NO_DEVICE);
		info.setPermissions(file->getPermissions());
		info.setSize(static_cast<size_t>(0u));
		info.setAccessTimestamp(file->getAccessTimestamp());
		info.setModificationTimestamp(file->getModificationTimestamp());
		info.setStatusChangeTimestamp(file->getStatusChangeTimestamp());
		file->stat(info);
		file.move();
	}

	void MemoryBase::chmod(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		file->chmod(permissions);
		file.move();
	}

	void MemoryBase::chown(PathIterator pathBegin, PathIterator pathEnd, Stat::UserID owner, bool ofLink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		if(!ofLink)
			file = snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
		file->chown(owner);
		file.move();
	}

	void MemoryBase::chgrp(PathIterator pathBegin, PathIterator pathEnd, Stat::GroupID group, bool ofLink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		if(!ofLink)
			file = snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
		file->chgrp(group);
		file.move();
	}

	void MemoryBase::link(PathIterator oldPathBegin, PathIterator oldPathEnd,
			PathIterator newPathBegin, PathIterator newPathEnd) {
		Ref<MemoryFile> source(requireFile(oldPathBegin, oldPathEnd));
		if(source->getFileType() == Stat::DIRECTORY) {
			source.move();
			throw IsADirectoryError(Transcode::bmpToUTF8(VFS::constructPathname(oldPathBegin, oldPathEnd, true)));
		}
		String16 basename;
		Ref<MemoryDirectory> destination(requireParentDirectory(newPathBegin, newPathEnd, &basename));
		ObjectLocker<MemoryFile> locker(getEffectiveMutexPool(), *destination);
		Ref<MemoryFile> present;
		if(!basename.empty())
			present = destination->getEntry(basename);
		if(basename.empty() || *present) {
			present.move();
			locker.release();
			source.move();
			destination.move();
			throw FileAlreadyExistsError(Transcode::bmpToUTF8(VFS::constructPathname(newPathBegin,
					newPathEnd, true)));
		}
		destination->putEntry(basename, *source);
		locker.release();
		source.move();
		destination.move();
	}

	void MemoryBase::unlink(PathIterator pathBegin, PathIterator pathEnd) {
		String16 basename;
		Ref<MemoryDirectory> parent(requireParentDirectory(pathBegin, pathEnd, &basename));
		if(basename.empty()) {
			parent.move();
			throw IsADirectoryError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		}
		ObjectLocker<MemoryFile> locker(getEffectiveMutexPool(), *parent);
		Ref<MemoryFile> child(parent->getEntry(basename));
		if(!*child)
			throw NoSuchFileError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		if(child->getFileType() == Stat::DIRECTORY) {
			locker.release();
			child.move();
			parent.move();
			throw IsADirectoryError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		}
		parent->removeEntry(basename);
		locker.release();
		parent.move();
		child.move();
	}

	void MemoryBase::utime(PathIterator pathBegin, PathIterator pathEnd) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		file = snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
		file->utime();
		file.move();
	}

	void MemoryBase::utime(PathIterator pathBegin, PathIterator pathEnd,
			time_t accessTimestamp, time_t modificationTimestamp) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		file = snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
		file->utime(accessTimestamp, modificationTimestamp);
		file.move();
	}

	bool MemoryBase::access(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		if(permissions == VFS::FILE_EXISTS) {
			PathIterator center(pathBegin);
			MemoryFile* file = resolvePath(center, pathEnd);
			if(!file)
				return false;
			file = snapSymbolicLinks(pathBegin, pathEnd, file, true);
			if(!file)
				return false;
			file->unref();
			return true;
		}
		else {
			try {
				Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
				file = snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
				bool allowed = file->access(permissions);
				file.move();
				return allowed;
			}
			catch(const AccessDeniedError&) {
				return false;
			}
		}
	}

	void MemoryBase::sync() {}

	void MemoryBase::rename(PathIterator oldPathBegin, PathIterator oldPathEnd,
			PathIterator newPathBegin, PathIterator newPathEnd) {
		String16 srcbase, destbase;
		Ref<MemoryDirectory> srcdir(requireParentDirectory(oldPathBegin, newPathBegin, &srcbase));
		ObjectLocker<MemoryFile> srclock(getEffectiveMutexPool(), *srcdir);
		Ref<MemoryFile> srcfile(srcdir->getEntry(srcbase));
		if(!*srcfile) {
			srcdir.move();
			srclock.release();
			throw NoSuchFileError(Transcode::bmpToUTF8(VFS::constructPathname(oldPathBegin, oldPathEnd, true)));
		}
		TreePath desttrace;
		Ref<MemoryDirectory> destdir(requireParentDirectory(newPathBegin, newPathEnd, &destbase, 0u, &desttrace));
		ObjectLocker<MemoryFile> destlock(getEffectiveMutexPool(), *destdir);
		bool loop = desttrace.contains(**srcfile);
		desttrace.clear();
		if(loop) {
			destdir.move();
			srcfile.move();
			srcdir.move();
			srclock.release();
			destlock.release();
			throw AttemptedDirectoryLoopError(Transcode::bmpToUTF8(VFS::constructPathname(oldPathBegin,
					oldPathEnd, true)));
		}
		MemoryFile* destfile = destdir->getEntry(destbase);
		if(destfile) {
			destfile->unref();
			destdir.move();
			srcfile.move();
			srcdir.move();
			srclock.release();
			destlock.release();
			throw FileAlreadyExistsError(Transcode::bmpToUTF8(VFS::constructPathname(newPathBegin,
					newPathEnd, true)));
		}
		destdir->putEntry(destbase, *srcfile);
		destlock.release();
		srcdir->removeEntry(srcbase);
		srclock.release();
		destdir.move();
		srcfile.move();
		srcdir.move();
	}

	void MemoryBase::mkdir(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		String16 basename;
		Ref<MemoryDirectory> parent(requireParentDirectory(pathBegin, pathEnd, &basename));
		ObjectLocker<MemoryFile> lock(getEffectiveMutexPool(), *parent);
		MemoryFile* child = parent->getEntry(basename);
		if(child) {
			child->unref();
			parent.move();
			lock.release();
			throw FileAlreadyExistsError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		}
		Ref<MemoryDirectory> newdir(createDirectory(permissions));
		parent->putEntry(basename, *newdir);
		newdir.move();
		parent.move();
		lock.release();
	}

	void MemoryBase::rmdir(PathIterator pathBegin, PathIterator pathEnd) {
		String16 basename;
		Ref<MemoryDirectory> parent(requireParentDirectory(pathBegin, pathEnd, &basename));
		if(basename.empty()) {
			if(!root->isEmptyDirectory())
				throw DirectoryNotEmptyError("/");
			parent.move();
			return;
		}
		ObjectLocker<MemoryFile> locker(getEffectiveMutexPool(), *parent);
		Ref<MemoryFile> child(parent->getEntry(basename));
		if(!*child)
			throw NoSuchFileError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		if(child->getFileType() != Stat::DIRECTORY) {
			locker.release();
			child.move();
			parent.move();
			throw NotADirectoryError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		}
		if(!static_cast<MemoryDirectory*>(*child)->isEmptyDirectory()) {
			locker.release();
			child.move();
			parent.move();
			throw DirectoryNotEmptyError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		}
		parent->removeEntry(basename);
		locker.release();
		parent.move();
		child.move();
	}

	void MemoryBase::symlink(const String16& target, PathIterator pathBegin, PathIterator pathEnd) {
		String16 basename;
		Ref<MemoryDirectory> parent(requireParentDirectory(pathBegin, pathEnd, &basename));
		ObjectLocker<MemoryFile> lock(getEffectiveMutexPool(), *parent);
		MemoryFile* child = parent->getEntry(basename);
		if(child) {
			child->unref();
			parent.move();
			lock.release();
			throw FileAlreadyExistsError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		}
		Ref<MemoryFile> newlink(createSymlink(target));
		parent->putEntry(basename, *newlink);
		newlink.move();
		parent.move();
		lock.release();
	}

	void MemoryBase::readlink(PathIterator pathBegin, PathIterator pathEnd, String16& result) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		file->readlink(result);
		file.move();
	}

	void MemoryBase::readdir(PathIterator pathBegin, PathIterator pathEnd, Appender<String16>& sink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		file = snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
		file.move();
		if(file->getFileType() != Stat::DIRECTORY) {
			file.move();
			throw NotADirectoryError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		}
		static_cast<MemoryDirectory*>(*file)->readdir(sink);
		file.move();
	}

	void MemoryBase::truncate(PathIterator pathBegin, PathIterator pathEnd, size_t size) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		file->truncate(size);
		file.move();
	}

	void MemoryBase::statfs(PathIterator pathBegin, PathIterator pathEnd, FSInfo& info) {
		requireFile(pathBegin, pathEnd)->unref();
		info.setType(RED_MEMORYFS);
		info.setTotalBlockCount(static_cast<size_t>(0u));
		info.setFreeBlockCount(static_cast<size_t>(0u));
		info.setTotalINodeCount(static_cast<size_t>(0u));
		info.setFreeINodeCount(static_cast<size_t>(0u));
		info.setMaximumFilenameLength(IntegerBounds<size_t>::MAX);
	}

	void MemoryBase::mknod(PathIterator pathBegin, PathIterator pathEnd, Stat::Type type, int permissions,
			Stat::DeviceID device) {
		String16 basename;
		Ref<MemoryDirectory> parent(requireParentDirectory(pathBegin, pathEnd, &basename));
		ObjectLocker<MemoryFile> lock(getEffectiveMutexPool(), *parent);
		MemoryFile* child = parent->getEntry(basename);
		if(child) {
			child->unref();
			parent.move();
			lock.release();
			throw FileAlreadyExistsError(Transcode::bmpToUTF8(VFS::constructPathname(pathBegin, pathEnd, true)));
		}
		Ref<MemoryFile> newchild;
		switch(type) {
			case Stat::REGULAR_FILE:
				newchild = createRegularFile(permissions);
				break;
			case Stat::CHARACTER_DEVICE:
				newchild = createDeviceFile(permissions, false, device);
				break;
			case Stat::BLOCK_DEVICE:
				newchild = createDeviceFile(permissions, true, device);
				break;
			default:
				throw UnsupportedFileTypeError(type);
		}
		parent->putEntry(basename, *newchild);
		newchild.move();
		parent.move();
		lock.release();
	}

	/*TODO
	InputStream<char>* MemoryBase::getInputStream(PathIterator, PathIterator);
	OutputStream<char>* MemoryBase::getOutputStream(PathIterator, PathIterator);
	BidirectionalStream<char>* MemoryBase::getStream(PathIterator, PathIterator, bool);
	VFile* MemoryBase::getFileReference(PathIterator, PathIterator);
	*/

}}

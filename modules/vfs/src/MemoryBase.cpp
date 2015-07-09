#include <ctime>
#include <redstrain/util/Ref.hpp>
#include <redstrain/text/Transcode.hpp>
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
#include "TooManySymbolicLinksError.hpp"
#include "AttemptedDirectoryLoopError.hpp"
#include "tweaks.hpp"

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
		sink.doneAppending();
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

	void MemoryBase::stat(PathIterator pathBegin, PathIterator pathEnd, Stat& info, bool ofLink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		if(!ofLink)
			file = MemoryBase::snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
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

	void MemoryBase::chmod(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		file->chmod(permissions);
		file.move();
	}

	void MemoryBase::chown(PathIterator pathBegin, PathIterator pathEnd, Stat::UserID owner, bool ofLink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		if(!ofLink)
			file = MemoryBase::snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
		file->chown(owner);
		file.move();
	}

	void MemoryBase::chgrp(PathIterator pathBegin, PathIterator pathEnd, Stat::GroupID group, bool ofLink) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		if(!ofLink)
			file = MemoryBase::snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
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
		file = MemoryBase::snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
		file->utime();
		file.move();
	}

	void MemoryBase::utime(PathIterator pathBegin, PathIterator pathEnd,
			time_t accessTimestamp, time_t modificationTimestamp) {
		Ref<MemoryFile> file(requireFile(pathBegin, pathEnd));
		file = MemoryBase::snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
		file->utime(accessTimestamp, modificationTimestamp);
		file.move();
	}

	bool MemoryBase::access(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		if(permissions == VFS::FILE_EXISTS) {
			PathIterator center(pathBegin);
			MemoryFile* file = MemoryBase::resolvePath(center, pathEnd);
			if(!file)
				return false;
			file = MemoryBase::snapSymbolicLinks(pathBegin, pathEnd, file, true);
			if(!file)
				return false;
			file->unref();
			return true;
		}
		else {
			try {
				Ref<MemoryFile> file(MemoryBase::requireFile(pathBegin, pathEnd));
				file = MemoryBase::snapSymbolicLinks(pathBegin, pathEnd, file.set(), false);
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

	/*
	void MemoryBase::symlink(PathIterator, PathIterator, const String16&);
	void MemoryBase::readlink(PathIterator, PathIterator, String16&);
	void MemoryBase::readdir(PathIterator, PathIterator, Appender<String16>&);
	void MemoryBase::truncate(PathIterator, PathIterator, size_t);
	void MemoryBase::statfs(PathIterator, PathIterator, FSInfo&);
	void MemoryBase::mknod(PathIterator, PathIterator, Stat::Type, int, Stat::DeviceID);
	InputStream<char>* MemoryBase::getInputStream(PathIterator, PathIterator);
	OutputStream<char>* MemoryBase::getOutputStream(PathIterator, PathIterator);
	BidirectionalStream<char>* MemoryBase::getStream(PathIterator, PathIterator, bool);
	VFile* MemoryBase::getFileReference(PathIterator, PathIterator);
	*/

}}

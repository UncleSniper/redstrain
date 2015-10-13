#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/io/MemoryInputStream.hpp>
#include <redstrain/platform/ObjectLocker.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/FormattedOutputStream.hpp>
#include <redstrain/io/MissingInputSeparatorError.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>

#include "BlobVFS.hpp"
#include "NotADirectoryError.hpp"
#include "FileAlreadyExistsError.hpp"
#include "ReadOnlyFilesystemError.hpp"

using std::string;
using redengine::util::Ref;
using redengine::util::Delete;
using redengine::text::String16;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::util::StringUtils;
using redengine::io::MemoryInputStream;
using redengine::platform::ObjectLocker;
using redengine::io::BidirectionalStream;
using redengine::io::FormattedInputStream;
using redengine::io::FormattedOutputStream;
using redengine::io::MissingInputSeparatorError;
using redengine::platform::SynchronizedSingleton;

namespace redengine {
namespace vfs {

	// ======== BlobMemoryFile ========

	BlobVFS::BlobMemoryFile::BlobMemoryFile(MemoryBase& fs, int permissions, const char* data, size_t size)
			: MemoryFile(fs, permissions), data(data), size(size) {}

	BlobVFS::BlobMemoryFile::BlobMemoryFile(const BlobMemoryFile& file)
			: MemoryFile(file), data(file.data), size(file.size) {}

	Stat::Type BlobVFS::BlobMemoryFile::getFileType() const {
		return Stat::REGULAR_FILE;
	}

	void BlobVFS::BlobMemoryFile::stat(Stat& info) {
		info.setSize(size);
	}

	void BlobVFS::BlobMemoryFile::truncate(size_t) {
		throw ReadOnlyFilesystemError();
	}

	InputStream<char>* BlobVFS::BlobMemoryFile::getInputStream() {
		return new MemoryInputStream<char>(data, size);
	}

	OutputStream<char>* BlobVFS::BlobMemoryFile::getOutputStream() {
		throw ReadOnlyFilesystemError();
	}

	BidirectionalStream<char>* BlobVFS::BlobMemoryFile::getStream(bool) {
		throw ReadOnlyFilesystemError();
	}

	// ======== BlobEmitter ========

	BlobVFS::BlobEmitter::BlobEmitter() {}

	BlobVFS::BlobEmitter::BlobEmitter(const BlobEmitter& emitter) : ReferenceCounted(emitter) {}

	// ======== BlobInjector ========

	BlobVFS::BlobInjector::BlobInjector(const char* data, size_t size, const string& path)
			: data(data), size(size), path(path) {
		BlobVFS::addBlobEmitter(this);
	}

	BlobVFS::BlobInjector::BlobInjector(const BlobInjector& injector)
			: BlobEmitter(injector), data(injector.data), size(injector.size), path(injector.path) {}

	void BlobVFS::BlobInjector::emitBlobs(BlobVFS& vfs) {
		vfs.putBlob(path, data, size);
	}

	// ======== BlobLinker ========

	BlobVFS::BlobLinker::BlobLinker(const string& oldPath, const string& newPath)
			: oldPath(oldPath), newPath(newPath) {}

	BlobVFS::BlobLinker::BlobLinker(const BlobLinker& linker)
			: BlobEmitter(linker), oldPath(linker.oldPath), newPath(linker.newPath) {}

	void BlobVFS::BlobLinker::emitBlobs(BlobVFS& vfs) {
		vfs.aliasBlob(oldPath, newPath);
	}

	// ======== BlobVFS ========

	BlobVFS::BlobEmitters* BlobVFS::emitters = NULL;

	BlobVFS::BlobVFS(MemoryDirectory* root, int baseFlags)
			: MemoryBase(root, baseFlags | MemoryBase::BFL_READONLY),
			defaultDirectoryPermissions(DEFAULT_DIRECTORY_PERMISSIONS) {}

	BlobVFS::BlobVFS(int baseFlags) : MemoryBase(NULL, baseFlags | MemoryBase::BFL_READONLY),
			defaultDirectoryPermissions(DEFAULT_DIRECTORY_PERMISSIONS) {}

	BlobVFS::BlobVFS(const BlobVFS& vfs) : MemoryBase(vfs),
			defaultDirectoryPermissions(vfs.defaultDirectoryPermissions) {}

	void BlobVFS::putBlob(const string& path, const char* data, size_t size) {
		Pathname parts;
		deconstructPathname(path, parts);
		putBlob(parts, data, size);
	}

	void BlobVFS::putBlob(const String16& path, const char* data, size_t size) {
		Pathname parts;
		VFS::deconstructPathname(path, parts);
		putBlob(parts, data, size);
	}

	void BlobVFS::putBlob(const Pathname& path, const char* data, size_t size) {
		if(path.empty())
			throw FileAlreadyExistsError("/");
		PathIterator begin(path.begin()), end(path.end());
		Ref<MemoryDirectory> current(getRoot(), true);
		String16 basename;
		ObjectLocker<MemoryFile> locker(getEffectiveMutexPool(), NULL);
		for(;;) {
			basename = *begin;
			if(++begin == end)
				break;
			locker.move(*current);
			Ref<MemoryFile> child(current->getEntry(basename));
			if(*child) {
				if(child->getFileType() != Stat::DIRECTORY) {
					child.move();
					current.move();
					locker.release();
					throw NotADirectoryError(path.begin(), begin);
				}
				current = static_cast<MemoryDirectory*>(child.set());
			}
			else {
				Ref<MemoryDirectory> newdir(createDirectory(defaultDirectoryPermissions));
				current->putEntry(basename, *newdir);
				current.move();
				current = newdir.set();
			}
			locker.release();
		}
		locker.move(*current);
		Ref<MemoryFile> blob(current->getEntry(basename));
		if(*blob) {
			blob.move();
			current.move();
			locker.release();
			throw FileAlreadyExistsError(path);
		}
		blob = new BlobMemoryFile(*this, getDefaultFilePermissions(), data, size);
		current->putEntry(basename, *blob);
		blob.move();
		current.move();
		locker.release();
	}

	void BlobVFS::aliasBlob(const string& oldPath, const string& newPath) {
		Pathname newpl;
		deconstructPathname(newPath, newpl);
		aliasBlob(decodePathname(oldPath), newpl);
	}

	void BlobVFS::aliasBlob(const String16& oldPath, const String16& newPath) {
		Pathname newpl;
		VFS::deconstructPathname(newPath, newpl);
		aliasBlob(oldPath, newpl);
	}

	void BlobVFS::aliasBlob(const String16& target, const Pathname& path) {
		if(path.empty())
			throw FileAlreadyExistsError("/");
		PathIterator begin(path.begin()), end(path.end());
		Ref<MemoryDirectory> current(getRoot(), true);
		String16 basename;
		ObjectLocker<MemoryFile> locker(getEffectiveMutexPool(), NULL);
		for(;;) {
			basename = *begin;
			if(++begin == end)
				break;
			locker.move(*current);
			Ref<MemoryFile> child(current->getEntry(basename));
			if(*child) {
				if(child->getFileType() != Stat::DIRECTORY) {
					child.move();
					current.move();
					locker.release();
					throw NotADirectoryError(path.begin(), begin);
				}
				current = static_cast<MemoryDirectory*>(child.set());
			}
			else {
				Ref<MemoryDirectory> newdir(createDirectory(defaultDirectoryPermissions));
				current->putEntry(basename, *newdir);
				current.move();
				current = newdir.set();
			}
			locker.release();
		}
		locker.move(*current);
		Ref<MemoryFile> link(current->getEntry(basename));
		if(*link) {
			link.move();
			current.move();
			locker.release();
			throw FileAlreadyExistsError(path);
		}
		link = createSymlink(target);
		current->putEntry(basename, *link);
		link.move();
		current.move();
		locker.release();
	}

	void BlobVFS::putEmittedBlobs() {
		if(!BlobVFS::emitters)
			return;
		BlobEmitters::const_iterator begin(BlobVFS::emitters->begin()), end(BlobVFS::emitters->end());
		for(; begin != end; ++begin)
			(*begin)->emitBlobs(*this);
	}

	MemoryBase::MemoryFile* BlobVFS::createRegularFile(int permissions) {
		return new BlobMemoryFile(*this, permissions, NULL, static_cast<size_t>(0u));
	}

	void BlobVFS::addBlobEmitter(BlobEmitter* emitter) {
		if(!emitter)
			return;
		if(!BlobVFS::emitters)
			BlobVFS::emitters = new BlobEmitters;
		if(BlobVFS::emitters->insert(emitter).second)
			emitter->ref();
	}

	void BlobVFS::removeBlobEmitter(BlobEmitter* emitter) {
		if(!emitter || !BlobVFS::emitters)
			return;
		BlobEmitters::iterator it = BlobVFS::emitters->find(emitter);
		if(it == BlobVFS::emitters->end())
			return;
		BlobVFS::emitters->erase(it);
		emitter->unref();
	}

	class BlobVFSSingleton : public SynchronizedSingleton<BlobVFS> {

	  protected:
		virtual BlobVFS* newInstance() {
			Delete<BlobVFS> vfs(new BlobVFS);
			vfs->putEmittedBlobs();
			return vfs.set();
		}

	  public:
		BlobVFSSingleton() {}
		BlobVFSSingleton(const BlobVFSSingleton& singleton) : SynchronizedSingleton<BlobVFS>(singleton) {}

	};

	static BlobVFSSingleton defaultBlobVFS;

	BlobVFS& BlobVFS::getDefaultBlobVFS() {
		return defaultBlobVFS.get();
	}

}}

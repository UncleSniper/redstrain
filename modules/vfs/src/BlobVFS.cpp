#include <redstrain/io/MemoryInputStream.hpp>
#include <redstrain/platform/ObjectLocker.hpp>

#include "BlobVFS.hpp"
#include "NotADirectoryError.hpp"
#include "FileAlreadyExistsError.hpp"
#include "ReadOnlyFilesystemError.hpp"

using std::string;
using redengine::util::Ref;
using redengine::text::String16;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::MemoryInputStream;
using redengine::platform::ObjectLocker;
using redengine::io::BidirectionalStream;

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

	// ======== BlobVFS ========

	BlobVFS::BlobEmitters BlobVFS::emitters;

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

	void BlobVFS::putEmittedBlobs() {
		BlobEmitterIterator begin(BlobVFS::emitters.begin()), end(BlobVFS::emitters.end());
		for(; begin != end; ++begin)
			(*begin)(*this);
	}

	MemoryBase::MemoryFile* BlobVFS::createRegularFile(int permissions) {
		return new BlobMemoryFile(*this, permissions, NULL, static_cast<size_t>(0u));
	}

	void BlobVFS::addBlobEmitter(BlobEmitter emitter) {
		BlobVFS::emitters.insert(emitter);
	}

	void BlobVFS::removeBlobEmitter(BlobEmitter emitter) {
		BlobVFS::emitters.erase(emitter);
	}

}}

#include <redstrain/io/MemoryInputStream.hpp>

#include "BlobVFS.hpp"
#include "IsADirectoryError.hpp"
#include "ReadOnlyFilesystemError.hpp"

using std::string;
using redengine::text::String16;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::MemoryInputStream;
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

	BlobVFS::BlobVFS(MemoryDirectory* root, int baseFlags)
			: MemoryBase(root, baseFlags | MemoryBase::BFL_READONLY) {}

	BlobVFS::BlobVFS(int baseFlags) : MemoryBase(NULL, baseFlags | MemoryBase::BFL_READONLY) {}

	BlobVFS::BlobVFS(const BlobVFS& vfs) : MemoryBase(vfs) {}

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

	/*TODO
	void BlobVFS::putBlob(const Pathname& path, const char* data, size_t size) {
		if(path.empty())
			throw IsADirectoryError("/");
		PathIterator begin(path.begin()), end(path.end());
		//
	}
	*/

	MemoryBase::MemoryFile* BlobVFS::createRegularFile(int permissions) {
		return new BlobMemoryFile(*this, permissions, NULL, static_cast<size_t>(0u));
	}

}}

#include "FileInputStream.hpp"

using std::string;
using redengine::platform::File;
using redengine::util::MemorySize;
using redengine::util::FileOffset;

namespace redengine {
namespace io {

	FileInputStream::FileInputStream(const FileInputStream& stream)
			: Stream(stream), FileBase(stream), InputStream<char>(stream) {}

	FileInputStream::FileInputStream(const string& path, File::Direction dir, bool create,
			File::TruncateMode truncate)
			: FileBase(path, dir == File::OUTPUT ? File::RANDOM_ACCESS : dir, create, truncate) {}

	FileInputStream::FileInputStream(const File& file) : FileBase(file) {}

	FileInputStream::FileInputStream(File::Handle handle, File::Direction dir)
			: FileBase(handle, dir == File::OUTPUT ? File::RANDOM_ACCESS : dir) {}

	MemorySize FileInputStream::readBlock(char* buffer, MemorySize size) {
		return file.read(buffer, size);
	}

	void FileInputStream::seek(FileOffset offset, SeekWhence whence) {
		FileBase::seek(offset, whence);
		atEnd = false;
	}

}}

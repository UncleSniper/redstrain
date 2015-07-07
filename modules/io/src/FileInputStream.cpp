#include "FileInputStream.hpp"

using std::string;
using redengine::platform::File;

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

	size_t FileInputStream::readBlock(char* buffer, size_t size) {
		return file.read(buffer, size);
	}

	void FileInputStream::seek(off_t offset, SeekWhence whence) {
		FileBase::seek(offset, whence);
		atEnd = false;
	}

}}

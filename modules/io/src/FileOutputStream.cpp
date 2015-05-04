#include "FileOutputStream.hpp"

using std::string;
using redengine::platform::File;

namespace redengine {
namespace io {

	FileOutputStream::FileOutputStream(const FileOutputStream& stream)
			: Stream(stream), FileBase(stream), OutputStream<char>(stream) {}

	FileOutputStream::FileOutputStream(const string& path, File::Direction dir, bool create)
			: FileBase(path, dir == File::INPUT ? File::RANDOM_ACCESS : dir, create) {}

	FileOutputStream::FileOutputStream(const File& file) : FileBase(file) {}

	FileOutputStream::FileOutputStream(File::Handle handle, File::Direction dir)
			: FileBase(handle, dir == File::INPUT ? File::RANDOM_ACCESS : dir) {}

	void FileOutputStream::writeBlock(const char* buffer, size_t size) {
		file.write(buffer, size);
	}

}}

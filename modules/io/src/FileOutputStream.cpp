#include "FileOutputStream.hpp"

using std::string;
using redengine::platform::File;
using redengine::util::MemorySize;

namespace redengine {
namespace io {

	FileOutputStream::FileOutputStream(const FileOutputStream& stream)
			: Stream(stream), FileBase(stream), OutputStream<char>(stream) {}

	FileOutputStream::FileOutputStream(const string& path, File::Direction dir, bool create,
			File::TruncateMode truncate)
			: FileBase(path, dir == File::INPUT ? File::RANDOM_ACCESS : dir, create, truncate) {}

	FileOutputStream::FileOutputStream(const File& file) : FileBase(file) {}

	FileOutputStream::FileOutputStream(File::Handle handle, File::Direction dir)
			: FileBase(handle, dir == File::INPUT ? File::RANDOM_ACCESS : dir) {}

	void FileOutputStream::writeBlock(const char* buffer, MemorySize size) {
		file.write(buffer, size);
	}

}}

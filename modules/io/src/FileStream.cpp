#include "FileStream.hpp"

using std::string;
using redengine::platform::File;

namespace redengine {
namespace io {

	FileStream::FileStream(const FileStream& stream)
			: Stream(stream), FileBase(stream), InputStream<char>(stream), FileInputStream(stream),
			OutputStream<char>(stream), FileOutputStream(stream), BidirectionalStream<char>(stream) {}

	FileStream::FileStream(const string& path, bool create)
			: FileBase(path, File::RANDOM_ACCESS, create),
			FileInputStream(path, File::RANDOM_ACCESS, create),
			FileOutputStream(path, File::RANDOM_ACCESS, create) {}

	FileStream::FileStream(const File& file) : FileBase(file), FileInputStream(file), FileOutputStream(file) {}

	FileStream::FileStream(File::Handle handle)
			: FileBase(handle, File::RANDOM_ACCESS),
			FileInputStream(handle, File::RANDOM_ACCESS),
			FileOutputStream(handle, File::RANDOM_ACCESS) {}

}}

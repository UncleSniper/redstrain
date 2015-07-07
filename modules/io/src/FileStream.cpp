#include "FileStream.hpp"

using std::string;
using redengine::platform::File;

namespace redengine {
namespace io {

	FileStream::FileStream(const FileStream& stream)
			: Stream(stream), FileBase(stream), InputStream<char>(stream), FileInputStream(stream),
			OutputStream<char>(stream), FileOutputStream(stream), BidirectionalStream<char>(stream) {}

	FileStream::FileStream(const string& path, bool create, File::TruncateMode truncate)
			: FileBase(path, File::RANDOM_ACCESS, create, truncate),
			FileInputStream(path, File::RANDOM_ACCESS, create, truncate),
			FileOutputStream(path, File::RANDOM_ACCESS, create, truncate) {}

	FileStream::FileStream(const File& file) : FileBase(file), FileInputStream(file), FileOutputStream(file) {}

	FileStream::FileStream(File::Handle handle)
			: FileBase(handle, File::RANDOM_ACCESS),
			FileInputStream(handle, File::RANDOM_ACCESS),
			FileOutputStream(handle, File::RANDOM_ACCESS) {}

}}

#include "VFile.hpp"

using std::string;
using redengine::text::String16;
using redengine::util::Appender;

namespace redengine {
namespace vfs {

	VFile::VFile(VFS& fs) : fs(fs) {}

	VFile::VFile(const VFile& file) : fs(file.fs) {}

	VFile::~VFile() {}

	void VFile::rename(const string& newPath) {
		VFS::Pathname newpl;
		fs.deconstructPathname(newPath, newpl);
		rename(newpl.begin(), newpl.end());
	}

	void VFile::rename(const String16& newPath) {
		VFS::Pathname newpl;
		VFS::deconstructPathname(newPath, newpl);
		rename(newpl.begin(), newpl.end());
	}

	void VFile::rename(const VFS::Pathname& newPath) {
		rename(newPath.begin(), newPath.end());
	}

	void VFile::readlink(string& result) {
		String16 result16;
		readlink(result16);
		result = fs.encodePathname(result16);
	}

	void VFile::readdir(Appender<string>& sink) {
		VFS::EncodingAppender encApp(sink, fs);
		readdir(encApp);
	}

}}

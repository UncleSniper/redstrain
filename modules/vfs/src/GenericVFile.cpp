#include "GenericVFile.hpp"

using std::string;
using redengine::text::String16;
using redengine::util::Appender;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::BidirectionalStream;

namespace redengine {
namespace vfs {

	GenericVFile::GenericVFile(VFS& fs, const string& path, bool ofLink) : VFile(fs), ofLink(ofLink) {
		fs.deconstructPathname(path, this->path);
	}

	GenericVFile::GenericVFile(VFS& fs, const String16& path, bool ofLink) : VFile(fs), ofLink(ofLink) {
		VFS::deconstructPathname(path, this->path);
	}

	GenericVFile::GenericVFile(VFS& fs, const VFS::Pathname& path, bool ofLink)
			: VFile(fs), path(path), ofLink(ofLink) {}

	GenericVFile::GenericVFile(VFS& fs, VFS::PathIterator pathBegin, VFS::PathIterator pathEnd, bool ofLink)
			: VFile(fs), ofLink(ofLink) {
		path.insert(path.begin(), pathBegin, pathEnd);
	}

	GenericVFile::GenericVFile(const GenericVFile& file) : VFile(file), path(file.path), ofLink(file.ofLink) {}

	void GenericVFile::stat(Stat& info) {
		getVFS().stat(path.begin(), path.end(), info, ofLink);
	}

	void GenericVFile::chmod(int permissions) {
		getVFS().chmod(path.begin(), path.end(), permissions);
	}

	void GenericVFile::chown(Stat::UserID owner) {
		getVFS().chown(path.begin(), path.end(), owner, ofLink);
	}

	void GenericVFile::chgrp(Stat::GroupID group) {
		getVFS().chgrp(path.begin(), path.end(), group, ofLink);
	}

	void GenericVFile::unlink() {
		getVFS().unlink(path.begin(), path.end());
	}

	void GenericVFile::utime() {
		getVFS().utime(path.begin(), path.end());
	}

	void GenericVFile::utime(time_t accessTimestamp, time_t modificationTimestamp) {
		getVFS().utime(path.begin(), path.end(), accessTimestamp, modificationTimestamp);
	}

	bool GenericVFile::access(int permissions) {
		return getVFS().access(path.begin(), path.end(), permissions);
	}

	void GenericVFile::rename(VFS::PathIterator newPathBegin, VFS::PathIterator newPathEnd) {
		getVFS().rename(path.begin(), path.end(), newPathBegin, newPathEnd);
		path.clear();
		path.insert(path.begin(), newPathBegin, newPathEnd);
	}

	void GenericVFile::mkdir(int permissions) {
		getVFS().mkdir(path.begin(), path.end(), permissions);
	}

	void GenericVFile::rmdir() {
		getVFS().rmdir(path.begin(), path.end());
	}

	void GenericVFile::readlink(String16& result) {
		getVFS().readlink(path.begin(), path.end(), result);
	}

	void GenericVFile::readdir(Appender<String16>& sink) {
		getVFS().readdir(path.begin(), path.end(), sink);
	}

	void GenericVFile::truncate(size_t size) {
		getVFS().truncate(path.begin(), path.end(), size);
	}

	void GenericVFile::statfs(VFS::FSInfo& info) {
		getVFS().statfs(path.begin(), path.end(), info);
	}

	void GenericVFile::mknod(Stat::Type type, int permissions, Stat::DeviceID device) {
		getVFS().mknod(path.begin(), path.end(), type, permissions, device);
	}

	InputStream<char>* GenericVFile::getInputStream() {
		return getVFS().getInputStream(path.begin(), path.end());
	}

	OutputStream<char>* GenericVFile::getOutputStream() {
		return getVFS().getOutputStream(path.begin(), path.end());
	}
	BidirectionalStream<char>* GenericVFile::getStream(bool truncate) {
		return getVFS().getStream(path.begin(), path.end(), truncate);
	}

}}

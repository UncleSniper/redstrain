#include <redstrain/util/Delete.hpp>

#include "RootedVFS.hpp"

using redengine::util::Delete;
using redengine::text::String16;
using redengine::util::Appender;
using redengine::util::FileSize;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::BidirectionalStream;

namespace redengine {
namespace vfs {

	// ======== RootedVFile ========

	RootedVFS::RootedVFile::RootedVFile(RootedVFS& vfs, VFile& vfile) : VFile(vfs), vfile(&vfile) {}

	RootedVFS::RootedVFile::RootedVFile(const RootedVFile& file) : VFile(file), vfile(file.vfile->cloneVFile()) {}

	RootedVFS::RootedVFile::~RootedVFile() {
		delete vfile;
	}

	void RootedVFS::RootedVFile::stat(Stat& info) {
		vfile->stat(info);
	}

	void RootedVFS::RootedVFile::chmod(int permissions) {
		vfile->chmod(permissions);
	}

	void RootedVFS::RootedVFile::chown(Stat::UserID owner) {
		vfile->chown(owner);
	}

	void RootedVFS::RootedVFile::chgrp(Stat::GroupID group) {
		vfile->chgrp(group);
	}

	void RootedVFS::RootedVFile::unlink() {
		vfile->unlink();
	}

	void RootedVFS::RootedVFile::utime() {
		vfile->utime();
	}

	void RootedVFS::RootedVFile::utime(time_t accessTimestamp, time_t modificationTimestamp) {
		vfile->utime(accessTimestamp, modificationTimestamp);
	}

	bool RootedVFS::RootedVFile::access(int permissions) {
		return vfile->access(permissions);
	}

	void RootedVFS::RootedVFile::rename(PathIterator newPathBegin, PathIterator newPathEnd) {
		Pathname newSlavePath;
		static_cast<RootedVFS&>(getVFS()).makeSlavePath(newPathBegin, newPathEnd, newSlavePath);
		vfile->rename(newSlavePath);
	}

	void RootedVFS::RootedVFile::mkdir(int permissions) {
		vfile->mkdir(permissions);
	}

	void RootedVFS::RootedVFile::rmdir() {
		vfile->rmdir();
	}

	void RootedVFS::RootedVFile::readlink(String16& result) {
		vfile->readlink(result);
	}

	void RootedVFS::RootedVFile::readdir(Appender<String16>& sink) {
		vfile->readdir(sink);
	}

	void RootedVFS::RootedVFile::truncate(FileSize size) {
		vfile->truncate(size);
	}

	void RootedVFS::RootedVFile::statfs(FSInfo& info) {
		vfile->statfs(info);
	}

	void RootedVFS::RootedVFile::mknod(Stat::Type type, int permissions, Stat::DeviceID device) {
		vfile->mknod(type, permissions, device);
	}

	InputStream<char>* RootedVFS::RootedVFile::getInputStream() {
		return vfile->getInputStream();
	}

	OutputStream<char>* RootedVFS::RootedVFile::getOutputStream() {
		return vfile->getOutputStream();
	}

	BidirectionalStream<char>* RootedVFS::RootedVFile::getStream(bool truncate) {
		return vfile->getStream(truncate);
	}

	VFile* RootedVFS::RootedVFile::cloneVFile() const {
		return new RootedVFile(*this);
	}

	// ======== RootedVFS ========

	RootedVFS::RootedVFS(VFS& slave, bool manageSlave, const Pathname& root)
			: slave(slave), manageSlave(manageSlave), root(root) {}

	RootedVFS::RootedVFS(VFS& slave, bool manageSlave, const String16& rootPath)
			: slave(slave), manageSlave(manageSlave) {
		VFS::deconstructPathname(rootPath, root);
	}

	RootedVFS::~RootedVFS() {
		if(manageSlave)
			delete &slave;
	}

	void RootedVFS::makeSlavePath(PathIterator pathBegin, PathIterator pathEnd, Pathname& pathname) const {
		pathname = root;
		for(; pathBegin != pathEnd; ++pathBegin)
			pathname.push_back(*pathBegin);
	}

	void RootedVFS::getRoot(PathIterator& begin, PathIterator& end) const {
		begin = root.begin();
		end = root.end();
	}

	void RootedVFS::stat(PathIterator pathBegin, PathIterator pathEnd, Stat& info, bool ofLink) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.stat(slavePath, info, ofLink);
	}

	void RootedVFS::chmod(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.chmod(slavePath, permissions);
	}

	void RootedVFS::chown(PathIterator pathBegin, PathIterator pathEnd, Stat::UserID owner, bool ofLink) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.chown(slavePath, owner, ofLink);
	}

	void RootedVFS::chgrp(PathIterator pathBegin, PathIterator pathEnd, Stat::GroupID group, bool ofLink) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.chgrp(slavePath, group, ofLink);
	}

	void RootedVFS::link(PathIterator oldPathBegin, PathIterator oldPathEnd,
			PathIterator newPathBegin, PathIterator newPathEnd) {
		Pathname oldSlavePath, newSlavePath;
		makeSlavePath(oldPathBegin, oldPathEnd, oldSlavePath);
		makeSlavePath(newPathBegin, newPathEnd, newSlavePath);
		slave.link(oldSlavePath, newSlavePath);
	}

	void RootedVFS::unlink(PathIterator pathBegin, PathIterator pathEnd) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.unlink(slavePath);
	}

	void RootedVFS::utime(PathIterator pathBegin, PathIterator pathEnd) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.utime(slavePath);
	}

	void RootedVFS::utime(PathIterator pathBegin, PathIterator pathEnd,
			time_t accessTimestamp, time_t modificationTimestamp) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.utime(slavePath, accessTimestamp, modificationTimestamp);
	}

	bool RootedVFS::access(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		return slave.access(slavePath, permissions);
	}

	void RootedVFS::sync() {
		slave.sync();
	}

	void RootedVFS::rename(PathIterator oldPathBegin, PathIterator oldPathEnd,
			PathIterator newPathBegin, PathIterator newPathEnd) {
		Pathname oldSlavePath, newSlavePath;
		makeSlavePath(oldPathBegin, oldPathEnd, oldSlavePath);
		makeSlavePath(newPathBegin, newPathEnd, newSlavePath);
		slave.rename(oldSlavePath, newSlavePath);
	}

	void RootedVFS::mkdir(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.mkdir(slavePath, permissions);
	}

	void RootedVFS::rmdir(PathIterator pathBegin, PathIterator pathEnd) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.rmdir(slavePath);
	}

	void RootedVFS::symlink(const String16& target, PathIterator pathBegin, PathIterator pathEnd) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.symlink(target, slavePath);
	}

	void RootedVFS::readlink(PathIterator pathBegin, PathIterator pathEnd, String16& result) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.readlink(slavePath, result);
	}

	void RootedVFS::readdir(PathIterator pathBegin, PathIterator pathEnd, Appender<String16>& sink) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.readdir(slavePath, sink);
	}

	void RootedVFS::truncate(PathIterator pathBegin, PathIterator pathEnd, FileSize size) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.truncate(slavePath, size);
	}

	void RootedVFS::statfs(PathIterator pathBegin, PathIterator pathEnd, FSInfo& info) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.statfs(slavePath, info);
	}

	void RootedVFS::mknod(PathIterator pathBegin, PathIterator pathEnd,
			Stat::Type type, int permissions, Stat::DeviceID device) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		slave.mknod(slavePath, type, permissions, device);
	}

	InputStream<char>* RootedVFS::getInputStream(PathIterator pathBegin, PathIterator pathEnd) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		return slave.getInputStream(slavePath);
	}

	OutputStream<char>* RootedVFS::getOutputStream(PathIterator pathBegin, PathIterator pathEnd) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		return slave.getOutputStream(slavePath);
	}

	BidirectionalStream<char>* RootedVFS::getStream(PathIterator pathBegin, PathIterator pathEnd, bool truncate) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		return slave.getStream(slavePath, truncate);
	}

	VFile* RootedVFS::getFileReference(PathIterator pathBegin, PathIterator pathEnd, bool ofLink) {
		Pathname slavePath;
		makeSlavePath(pathBegin, pathEnd, slavePath);
		Delete<VFile> slaveReference(slave.getFileReference(slavePath, ofLink));
		RootedVFile* rootedReference = new RootedVFile(*this, **slaveReference);
		slaveReference.set();
		return rootedReference;
	}

}}

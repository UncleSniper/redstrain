#include <redstrain/util/Ref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Encoder16.hpp>
#include <redstrain/text/UTF8Decoder16.hpp>
#include <redstrain/text/DefaultCodecFactory.hpp>

#include "HostVFS.hpp"
#include "FileInfoMapper.hpp"

using std::string;
using redengine::util::Ref;
using redengine::util::Delete;
using redengine::util::Appender;
using redengine::text::String16;
using redengine::text::Transcode;
using redengine::text::Encoder16;
using redengine::text::Decoder16;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::text::CodecFactory;
using redengine::text::UTF8Encoder16;
using redengine::text::UTF8Decoder16;
using redengine::platform::Filesystem;
using redengine::io::BidirectionalStream;
using redengine::text::DefaultCodecFactory;
typedef redengine::platform::Stat PlatformStat;

namespace redengine {
namespace vfs {

	HostVFS::HostVFS() {}

	HostVFS::HostVFS(const HostVFS& vfs)
			: VFS(vfs), hostEncoderFactory(vfs.hostEncoderFactory), hostDecoderFactory(vfs.hostDecoderFactory) {
		if(hostEncoderFactory)
			hostEncoderFactory->ref();
		if(hostDecoderFactory)
			hostDecoderFactory->ref();
	}

	HostVFS::~HostVFS() {
		if(hostEncoderFactory)
			hostEncoderFactory->unref();
		if(hostDecoderFactory)
			hostDecoderFactory->unref();
	}

	void HostVFS::setHostPathnameEncoderFactory(CodecFactory<Encoder16>* encoderFactory) {
		Ref<CodecFactory<Encoder16> > factory(encoderFactory, true);
		if(this->hostEncoderFactory)
			this->hostEncoderFactory->unref();
		this->hostEncoderFactory = encoderFactory;
		factory.set();
	}

	void HostVFS::setHostPathnameDecoderFactory(CodecFactory<Decoder16>* decoderFactory) {
		Ref<CodecFactory<Decoder16> > factory(decoderFactory, true);
		if(this->hostDecoderFactory)
			this->hostDecoderFactory->unref();
		this->hostDecoderFactory = decoderFactory;
		factory.set();
	}

	void HostVFS::setFileInfoMapper(FileInfoMapper* mapper) {
		Ref<FileInfoMapper> m(mapper, true);
		if(infoMapper)
			infoMapper->unref();
		infoMapper = mapper;
		m.set();
	}

	static DefaultCodecFactory<Decoder16, UTF8Decoder16> defaultDecoderFactory;

	String16 HostVFS::decodeHostPathname(const string& pathname) const {
		Delete<Decoder16> decoder((hostDecoderFactory ? hostDecoderFactory : &defaultDecoderFactory)->newCodec());
		String16 result;
		Transcode::decode(pathname, result, **decoder);
		return result;
	}

	static DefaultCodecFactory<Encoder16, UTF8Encoder16> defaultEncoderFactory;

	string HostVFS::encodeHostPathname(const String16& pathname) const {
		Delete<Encoder16> encoder((hostEncoderFactory ? hostEncoderFactory : &defaultEncoderFactory)->newCodec());
		string result;
		Transcode::encode(pathname, result, **encoder);
		return result;
	}

	void HostVFS::sync() {
		Filesystem::sync();
	}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	#define toHostPath(begin, end) (encodeHostPathname(VFS::constructPathname((begin), (end), true)))

	void HostVFS::stat(PathIterator pathBegin, PathIterator pathEnd, Stat& info, bool ofLink) {
		PlatformStat pinfo;
		Filesystem::stat(toHostPath(pathBegin, pathEnd), pinfo, ofLink);
		//TODO
	}

	/*
	void HostVFS::chmod(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		//TODO
	}

	void HostVFS::chown(PathIterator pathBegin, PathIterator pathEnd, Stat::UserID owner, bool ofLink) {
		//TODO
	}

	void HostVFS::chgrp(PathIterator pathBegin, PathIterator pathEnd, Stat::GroupID group, bool ofLink) {
		//TODO
	}

	void HostVFS::link(PathIterator oldPathBegin, PathIterator oldPathEnd,
			PathIterator newPathBegin, PathIterator newPathEnd) {
		//TODO
	}

	void HostVFS::unlink(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	void HostVFS::utime(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	void HostVFS::utime(PathIterator pathBegin, PathIterator pathEnd,
			time_t accessTimestamp, time_t modificationTimestamp) {
		//TODO
	}

	bool HostVFS::access(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		//TODO
	}

	void HostVFS::rename(PathIterator oldPathBegin, PathIterator oldPathEnd,
			PathIterator newPathBegin, PathIterator newPathEnd) {
		//TODO
	}

	void HostVFS::mkdir(PathIterator pathBegin, PathIterator pathBegin, int permissions) {
		//TODO
	}

	void HostVFS::rmdir(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	void HostVFS::symlink(PathIterator oldPathBegin, PathIterator oldPathEnd, const String16& newPath) {
		//TODO
	}

	void HostVFS::readlink(PathIterator pathBegin, PathIterator pathEnd, String16& result) {
		//TODO
	}

	void HostVFS::readdir(PathIterator pathBegin, PathIterator pathEnd, Appender<String16>& sink) {
		//TODO
	}

	void HostVFS::truncate(PathIterator pathBegin, PathIterator pathEnd, size_t size) {
		//TODO
	}

	void HostVFS::statfs(PathIterator pathBegin, PathIterator pathEnd, FSInfo& info) {
		//TODO
	}

	void HostVFS::mknod(PathIterator pathBegin, PathIterator pathEnd, Stat::Type type,
			int permissions, Stat::DeviceID device) {
		//TODO
	}

	InputStream<char>* HostVFS::getInputStream(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	OutputStream<char>* HostVFS::getOutputStream(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	BidirectionalStream<char>* HostVFS::getStream(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}
	*/

	#undef toHostPath

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

#error Not implemented yet

	void HostVFS::stat(PathIterator pathBegin, PathIterator pathEnd, Stat& info, bool ofLink) {
		//TODO
	}

	void HostVFS::chmod(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		//TODO
	}

	void HostVFS::chown(PathIterator pathBegin, PathIterator pathEnd, Stat::UserID owner, bool ofLink) {
		//TODO
	}

	void HostVFS::chgrp(PathIterator pathBegin, PathIterator pathEnd, Stat::GroupID group, bool ofLink) {
		//TODO
	}

	void HostVFS::link(PathIterator oldPathBegin, PathIterator oldPathEnd,
			PathIterator newPathBegin, PathIterator newPathEnd) {
		//TODO
	}

	void HostVFS::unlink(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	void HostVFS::utime(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	void HostVFS::utime(PathIterator pathBegin, PathIterator pathEnd,
			time_t accessTimestamp, time_t modificationTimestamp) {
		//TODO
	}

	bool HostVFS::access(PathIterator pathBegin, PathIterator pathEnd, int permissions) {
		//TODO
	}

	void HostVFS::rename(PathIterator oldPathBegin, PathIterator oldPathEnd,
			PathIterator newPathBegin, PathIterator newPathEnd) {
		//TODO
	}

	void HostVFS::mkdir(PathIterator pathBegin, PathIterator pathBegin, int permissions) {
		//TODO
	}

	void HostVFS::rmdir(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	void HostVFS::symlink(PathIterator oldPathBegin, PathIterator oldPathEnd, const String16& newPath) {
		//TODO
	}

	void HostVFS::readlink(PathIterator pathBegin, PathIterator pathEnd, String16& result) {
		//TODO
	}

	void HostVFS::readdir(PathIterator pathBegin, PathIterator pathEnd, Appender<String16>& sink) {
		//TODO
	}

	void HostVFS::truncate(PathIterator pathBegin, PathIterator pathEnd, size_t size) {
		//TODO
	}

	void HostVFS::statfs(PathIterator pathBegin, PathIterator pathEnd, FSInfo& info) {
		//TODO
	}

	void HostVFS::mknod(PathIterator pathBegin, PathIterator pathEnd, Stat::Type type,
			int permissions, Stat::DeviceID device) {
		//TODO
	}

	InputStream<char>* HostVFS::getInputStream(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	OutputStream<char>* HostVFS::getOutputStream(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

	BidirectionalStream<char>* HostVFS::getStream(PathIterator pathBegin, PathIterator pathEnd) {
		//TODO
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}
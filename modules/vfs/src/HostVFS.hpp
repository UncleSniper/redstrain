#ifndef REDSTRAIN_MOD_VFS_HOSTVFS_HPP
#define REDSTRAIN_MOD_VFS_HOSTVFS_HPP

#include "VFS.hpp"

namespace redengine {
namespace vfs {

	class FileInfoMapper;

	class REDSTRAIN_VFS_API HostVFS : public VFS {

	  private:
		text::CodecFactory<text::Encoder16>* hostEncoderFactory;
		text::CodecFactory<text::Decoder16>* hostDecoderFactory;
		FileInfoMapper* infoMapper;

	  public:
		HostVFS();
		HostVFS(const HostVFS&);
		virtual ~HostVFS();

		inline text::CodecFactory<text::Encoder16>* getHostPathnameEncoderFactory() const {
			return hostEncoderFactory;
		}

		void setHostPathnameEncoderFactory(text::CodecFactory<text::Encoder16>*);

		inline text::CodecFactory<text::Decoder16>* getHostPathnameDecoderFactory() const {
			return hostDecoderFactory;
		}

		void setHostPathnameDecoderFactory(text::CodecFactory<text::Decoder16>*);

		inline FileInfoMapper* getFileInfoMapper() const {
			return infoMapper;
		}

		void setFileInfoMapper(FileInfoMapper*);

		text::String16 decodeHostPathname(const std::string&) const;
		std::string encodeHostPathname(const text::String16&) const;

		virtual void stat(PathIterator, PathIterator, Stat&, bool);
		virtual void chmod(PathIterator, PathIterator, int);
		virtual void chown(PathIterator, PathIterator, Stat::UserID, bool);
		virtual void chgrp(PathIterator, PathIterator, Stat::GroupID, bool);
		virtual void link(PathIterator, PathIterator, PathIterator, PathIterator);
		virtual void unlink(PathIterator, PathIterator);
		virtual void utime(PathIterator, PathIterator);
		virtual void utime(PathIterator, PathIterator, time_t, time_t);
		virtual bool access(PathIterator, PathIterator, int);
		virtual void sync();
		virtual void rename(PathIterator, PathIterator, PathIterator, PathIterator);
		virtual void mkdir(PathIterator, PathIterator, int);
		virtual void rmdir(PathIterator, PathIterator);
		virtual void symlink(PathIterator, PathIterator, const text::String16&);
		virtual void readlink(PathIterator, PathIterator, text::String16&);
		virtual void readdir(PathIterator, PathIterator, util::Appender<text::String16>&);
		virtual void truncate(PathIterator, PathIterator, size_t);
		virtual void statfs(PathIterator, PathIterator, FSInfo&);
		virtual void mknod(PathIterator, PathIterator, Stat::Type, int, Stat::DeviceID);
		virtual io::InputStream<char>* getInputStream(PathIterator, PathIterator);
		virtual io::OutputStream<char>* getOutputStream(PathIterator, PathIterator);
		virtual io::BidirectionalStream<char>* getStream(PathIterator, PathIterator, bool);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_HOSTVFS_HPP */

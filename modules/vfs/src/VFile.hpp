#ifndef REDSTRAIN_MOD_VFS_VFILE_HPP
#define REDSTRAIN_MOD_VFS_VFILE_HPP

#include "VFS.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API VFile {

	  private:
		VFS& fs;

	  public:
		VFile(VFS&);
		VFile(const VFile&);
		virtual ~VFile();

		inline VFS& getVFS() {
			return fs;
		}

		inline const VFS& getVFS() const {
			return fs;
		}

		void rename(const std::string&);
		void rename(const text::String16&);
		void rename(const VFS::Pathname&);
		void readlink(std::string&);
		void readdir(util::Appender<std::string>&);

		virtual void stat(Stat&) = 0;
		virtual void chmod(int) = 0;
		virtual void chown(Stat::UserID) = 0;
		virtual void chgrp(Stat::GroupID) = 0;
		virtual void unlink() = 0;
		virtual void utime() = 0;
		virtual void utime(time_t, time_t) = 0;
		virtual bool access(int) = 0;
		virtual void rename(VFS::PathIterator, VFS::PathIterator) = 0;
		virtual void mkdir(int) = 0;
		virtual void rmdir() = 0;
		virtual void readlink(text::String16&) = 0;
		virtual void readdir(util::Appender<text::String16>&) = 0;
		virtual void truncate(size_t) = 0;
		virtual void statfs(VFS::FSInfo&) = 0;
		virtual void mknod(Stat::Type, int, Stat::DeviceID) = 0;
		virtual io::InputStream<char>* getInputStream() = 0;
		virtual io::OutputStream<char>* getOutputStream() = 0;
		virtual io::BidirectionalStream<char>* getStream(bool) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_VFILE_HPP */

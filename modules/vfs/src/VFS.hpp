#ifndef REDSTRAIN_MOD_VFS_VFS_HPP
#define REDSTRAIN_MOD_VFS_VFS_HPP

#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/text/types.hpp>

#include "Stat.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API VFS {

	  protected:
		VFS(const VFS&);

	  public:
		VFS();
		virtual ~VFS();

		virtual void stat(const std::string&, Stat&, bool) = 0;
		virtual void stat(const text::String16&, Stat&, bool) = 0;
		virtual void chmod(const std::string&, int) = 0;
		virtual void chmod(const text::String16&, int) = 0;
		virtual void chown(const std::string&, UserID, bool) = 0;
		virtual void chown(const text::String16&, UserID, bool) = 0;
		virtual void chgrp(const std::string&, GroupID, bool) = 0;
		virtual void chgrp(const text::String16&, GroupID, bool) = 0;
		virtual void link(const std::string&, const std::string&) = 0;
		virtual void unlink(const std::string&) = 0;
		virtual void utime(const std::string&) = 0;
		virtual void utime(const text::String16&) = 0;
		virtual void utime(const std::string&, time_t, time_t) = 0;
		virtual void utime(const text::String16&, time_t, time_t) = 0;
		virtual bool access(const std::string&, int) = 0;
		virtual bool access(const text::String16&, int) = 0;
		virtual void sync() = 0;
		virtual void rename(const std::string&, const std::string&) = 0;
		virtual void rename(const text::String16&, const text::String16&) = 0;
		virtual void mkdir(const std::string&, int) = 0;
		virtual void mkdir(const text::String16&, int) = 0;
		virtual void rmdir(const std::string&) = 0;
		virtual void rmdir(const text::String16&) = 0;
		virtual void symlink(const std::string&, const std::string&) = 0;
		virtual void symlink(const text::String16&, const text::String16&) = 0;
		virtual void readlink(const std::string&, std::string&) = 0;
		virtual void readlink(const text::String16&, text::String16&) = 0;
		virtual void readdir(const std::string&, util::Appender<std::string>&) = 0;
		virtual void readdir(const text::String16&, util::Appender<text::String16>&) = 0;
		virtual void truncate(const std::string&, size_t) = 0;
		virtual void truncate(const text::String16&, size_t) = 0;
		virtual void statfs(const std::string&, FSInfo&) = 0;
		virtual void statfs(const text::String16&, FSInfo&) = 0;
		virtual void mknod(const std::string&, Stat::Type, int, Stat::DeviceID) = 0;
		virtual void mknod(const text::String16&, Stat::Type, int, Stat::DeviceID) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_VFS_HPP */

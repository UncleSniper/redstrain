#ifndef REDSTRAIN_MOD_VFS_GENERICVFILE_HPP
#define REDSTRAIN_MOD_VFS_GENERICVFILE_HPP

#include "VFile.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API GenericVFile : public VFile {

	  private:
		VFS::Pathname path;
		bool ofLink;

	  public:
		GenericVFile(VFS&, const std::string&, bool);
		GenericVFile(VFS&, const text::String16&, bool);
		GenericVFile(VFS&, const VFS::Pathname&, bool);
		GenericVFile(VFS&, VFS::PathIterator, VFS::PathIterator, bool);
		GenericVFile(const GenericVFile&);

		inline const VFS::Pathname getPath() const {
			return path;
		}

		virtual void stat(Stat&);
		virtual void chmod(int);
		virtual void chown(Stat::UserID);
		virtual void chgrp(Stat::GroupID);
		virtual void unlink();
		virtual void utime();
		virtual void utime(time_t, time_t);
		virtual bool access(int);
		virtual void rename(VFS::PathIterator, VFS::PathIterator);
		virtual void mkdir(int);
		virtual void rmdir();
		virtual void readlink(text::String16&);
		virtual void readdir(util::Appender<text::String16>&);
		virtual void truncate(size_t);
		virtual void statfs(VFS::FSInfo&);
		virtual void mknod(Stat::Type, int, Stat::DeviceID);
		virtual io::InputStream<char>* getInputStream();
		virtual io::OutputStream<char>* getOutputStream();
		virtual io::BidirectionalStream<char>* getStream(bool);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_GENERICVFILE_HPP */

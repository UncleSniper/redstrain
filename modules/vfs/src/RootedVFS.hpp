#ifndef REDSTRAIN_MOD_VFS_ROOTEDVFS_HPP
#define REDSTRAIN_MOD_VFS_ROOTEDVFS_HPP

#include "VFS.hpp"
#include "VFile.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API RootedVFS : public VFS {

	  private:
		class REDSTRAIN_VFS_API RootedVFile : public VFile {

		  private:
			VFile* vfile;

		  public:
			RootedVFile(RootedVFS&, VFile&);
			RootedVFile(const RootedVFile&);
			virtual ~RootedVFile();

			virtual void stat(Stat&);
			virtual void chmod(int);
			virtual void chown(Stat::UserID);
			virtual void chgrp(Stat::GroupID);
			virtual void unlink();
			virtual void utime();
			virtual void utime(time_t, time_t);
			virtual bool access(int);
			virtual void rename(PathIterator, PathIterator);
			virtual void mkdir(int);
			virtual void rmdir();
			virtual void readlink(text::String16&);
			virtual void readdir(util::Appender<text::String16>&);
			virtual void truncate(util::FileSize);
			virtual void statfs(FSInfo&);
			virtual void mknod(Stat::Type, int, Stat::DeviceID);
			virtual io::InputStream<char>* getInputStream();
			virtual io::OutputStream<char>* getOutputStream();
			virtual io::BidirectionalStream<char>* getStream(bool);
			virtual VFile* cloneVFile() const;

		};

	  private:
		VFS& slave;
		bool manageSlave;
		Pathname root;

	  private:
		void makeSlavePath(PathIterator, PathIterator, Pathname&) const;

	  public:
		RootedVFS(VFS&, bool, const Pathname&);
		RootedVFS(VFS&, bool, const text::String16&);
		virtual ~RootedVFS();

		inline VFS& getSlave() {
			return slave;
		}

		inline const VFS& getSlave() const {
			return slave;
		}

		inline bool isManageSlave() const {
			return manageSlave;
		}

		inline void setManageSlave(bool manageSlave) {
			this->manageSlave = manageSlave;
		}

		inline const Pathname& getRoot() const {
			return root;
		}

		void getRoot(PathIterator&, PathIterator&) const;

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
		virtual void symlink(const text::String16&, PathIterator, PathIterator);
		virtual void readlink(PathIterator, PathIterator, text::String16&);
		virtual void readdir(PathIterator, PathIterator, util::Appender<text::String16>&);
		virtual void truncate(PathIterator, PathIterator, util::FileSize);
		virtual void statfs(PathIterator, PathIterator, FSInfo&);
		virtual void mknod(PathIterator, PathIterator, Stat::Type, int, Stat::DeviceID);
		virtual io::InputStream<char>* getInputStream(PathIterator, PathIterator);
		virtual io::OutputStream<char>* getOutputStream(PathIterator, PathIterator);
		virtual io::BidirectionalStream<char>* getStream(PathIterator, PathIterator, bool);
		virtual VFile* getFileReference(PathIterator, PathIterator, bool);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_ROOTEDVFS_HPP */

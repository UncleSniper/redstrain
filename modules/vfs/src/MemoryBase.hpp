#ifndef REDSTRAIN_MOD_VFS_MEMORYVFSBASE_HPP
#define REDSTRAIN_MOD_VFS_MEMORYVFSBASE_HPP

#include <map>
#include <redstrain/util/Ref.hpp>
#include <redstrain/platform/MutexPool.hpp>
#include <redstrain/util/ReferenceCounted.hpp>

#include "VFS.hpp"
#include "VFile.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API MemoryBase : public VFS {

	  public:
		class REDSTRAIN_VFS_API MemoryFile : private util::ReferenceCounted {

		  private:
			MemoryBase& fs;
			Stat::UserID owner;
			Stat::GroupID group;
			int permissions;
			time_t atime, mtime, ctime;

		  protected:
			void requireOwner() const;
			void requirePermissions(int);

		  public:
			MemoryFile(MemoryBase&, int);
			MemoryFile(const MemoryFile&);
			virtual ~MemoryFile();

			inline MemoryBase& getMemoryBase() {
				return fs;
			}

			inline const MemoryBase& getMemoryBase() const {
				return fs;
			}

			inline Stat::UserID getOwner() const {
				return owner;
			}

			inline void setOwner(Stat::UserID owner) {
				this->owner = owner;
			}

			inline Stat::GroupID getGroup() const {
				return group;
			}

			inline void setGroup(Stat::GroupID group) {
				this->group = group;
			}

			inline int getPermissions() const {
				return permissions;
			}

			inline void setPermissions(int permissions) {
				this->permissions = permissions;
			}

			inline time_t getAccessTimestamp() const {
				return atime;
			}

			inline void setAccessTimestamp(time_t newTimestamp) {
				atime = newTimestamp;
			}

			inline time_t getModificationTimestamp() const {
				return mtime;
			}

			inline void setModificationTimestamp(time_t newTimestamp) {
				mtime = newTimestamp;
			}

			inline time_t getStatusChangeTimestamp() const {
				return ctime;
			}

			inline void setStatusChangeTimestamp(time_t newTimestamp) {
				ctime = newTimestamp;
			}

			void ref();
			void unref();

			void genericStat(Stat&) const;

			virtual Stat::Type getFileType() const = 0;
			virtual void stat(Stat&) = 0;
			virtual void chmod(int);
			virtual void chown(Stat::UserID);
			virtual void chgrp(Stat::GroupID);
			virtual void utime();
			virtual void utime(time_t, time_t);
			virtual bool access(int);
			virtual void readlink(text::String16&);
			virtual void truncate(size_t) = 0;
			virtual io::InputStream<char>* getInputStream() = 0;
			virtual io::OutputStream<char>* getOutputStream() = 0;
			virtual io::BidirectionalStream<char>* getStream(bool) = 0;

		};

		class REDSTRAIN_VFS_API MemoryDirectory : public MemoryFile {

		  public:
			MemoryDirectory(MemoryBase&, int);
			MemoryDirectory(const MemoryDirectory&);

			void removeEntry(const text::String16&);

			virtual Stat::Type getFileType() const;
			virtual void truncate(size_t);
			virtual io::InputStream<char>* getInputStream();
			virtual io::OutputStream<char>* getOutputStream();
			virtual io::BidirectionalStream<char>* getStream(bool);
			virtual void readdir(util::Appender<text::String16>&) = 0;
			virtual void copydir(MemoryDirectory&) const = 0;
			virtual MemoryFile* getEntry(const text::String16&) = 0;
			virtual void putEntry(const text::String16&, MemoryFile*) = 0;
			virtual bool isEmptyDirectory() const = 0;

		};

		class REDSTRAIN_VFS_API SimpleMemoryDirectory : public MemoryDirectory {

		  private:
			typedef std::map<text::String16, MemoryFile*> EntryMap;
			typedef EntryMap::iterator EntryIterator;
			typedef EntryMap::const_iterator ConstEntryIterator;

		  private:
			EntryMap entries;

		  public:
			SimpleMemoryDirectory(MemoryBase&, int);
			SimpleMemoryDirectory(const SimpleMemoryDirectory&);
			virtual ~SimpleMemoryDirectory();

			virtual void stat(Stat&);
			virtual void readdir(util::Appender<text::String16>&);
			virtual void copydir(MemoryDirectory&) const;
			virtual MemoryFile* getEntry(const text::String16&);
			virtual void putEntry(const text::String16&, MemoryFile*);
			virtual bool isEmptyDirectory() const;

		};

		class REDSTRAIN_VFS_API SimpleMemorySymlink : public MemoryFile {

		  private:
			text::String16 target;

		  public:
			SimpleMemorySymlink(MemoryBase&, int, const text::String16&);
			SimpleMemorySymlink(const SimpleMemorySymlink&);

			virtual Stat::Type getFileType() const;
			virtual void stat(Stat&);
			virtual void readlink(text::String16&);
			virtual void truncate(size_t);
			virtual io::InputStream<char>* getInputStream();
			virtual io::OutputStream<char>* getOutputStream();
			virtual io::BidirectionalStream<char>* getStream(bool);

		};

		class REDSTRAIN_VFS_API SimpleMemoryDeviceFile : public MemoryFile {

		  private:
			bool block;
			Stat::DeviceID device;

		  public:
			SimpleMemoryDeviceFile(MemoryBase&, int, bool, Stat::DeviceID);
			SimpleMemoryDeviceFile(const SimpleMemoryDeviceFile&);

			virtual Stat::Type getFileType() const;
			virtual void stat(Stat&);
			virtual void truncate(size_t);
			virtual io::InputStream<char>* getInputStream();
			virtual io::OutputStream<char>* getOutputStream();
			virtual io::BidirectionalStream<char>* getStream(bool);

		};

		class REDSTRAIN_VFS_API TreePath {

		  private:
			std::list<MemoryFile*> stack;

		  public:
			TreePath();
			TreePath(const TreePath&);
			~TreePath();

			void pushFile(MemoryFile&);
			void clear();
			bool contains(MemoryFile&) const;

		};

		class REDSTRAIN_VFS_API MemoryVFile : public VFile {

		  private:
			text::String16 basename;
			Pathname fullpath;
			util::Ref<MemoryDirectory> parent;
			util::Ref<MemoryFile> child;

		  protected:
			MemoryFile* getOrMakeForOpen();

		  public:
			MemoryVFile(MemoryDirectory*, const text::String16&, MemoryFile*, const Pathname&);
			MemoryVFile(const MemoryVFile&);

			inline MemoryDirectory* getParentDirectory() const {
				return *parent;
			}

			inline MemoryFile* getChildFile() const {
				return *child;
			}

			inline const text::String16& getChildBasename() const {
				return basename;
			}

			inline const Pathname& getFullPathname() const {
				return fullpath;
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
			virtual void statfs(FSInfo&);
			virtual void mknod(Stat::Type, int, Stat::DeviceID);
			virtual io::InputStream<char>* getInputStream();
			virtual io::OutputStream<char>* getOutputStream();
			virtual io::BidirectionalStream<char>* getStream(bool);

		};

	  public:
		enum BaseFlags {
			BFL_HEED_OWNER          = 01,
			BFL_HEED_GROUP          = 02,
			BFL_ENFORCE_PERMISSIONS = 04
		};

	  public:
		static const int DIRECTORY_SEARCH_PERMISSIONS = VFS::CAN_EXECUTE;
		static const int DEFAULT_FILE_PERMISSIONS = 0644;

	  private:
		MemoryDirectory* root;
		platform::MutexPool* mutexPool;
		Stat::UserID currentUser;
		Stat::GroupID currentGroup;
		int flags, defaultPermissions;

	  protected:
		MemoryFile* resolvePath(PathIterator&, PathIterator, unsigned = 0u, TreePath* = NULL) const;
		MemoryFile* requireFile(PathIterator, PathIterator, unsigned = 0u, TreePath* = NULL) const;
		MemoryDirectory* requireParentDirectory(PathIterator, PathIterator,
				text::String16* = NULL, unsigned = 0u, TreePath* = NULL) const;
		MemoryFile* snapSymbolicLinks(PathIterator, PathIterator, MemoryFile*, bool,
				Pathname* = NULL, unsigned = 0u, TreePath* = NULL) const;
		MemoryFile* getOrMakeForOpen(const PathIterator&, const PathIterator&);
		void statfs(FSInfo&) const;

	  public:
		MemoryBase(MemoryDirectory*, int);
		MemoryBase(const MemoryBase&);
		virtual ~MemoryBase();

		inline MemoryDirectory* getRoot() const {
			return root;
		}

		inline platform::MutexPool* getMutexPool() const {
			return mutexPool;
		}

		inline void setMutexPool(platform::MutexPool* pool) {
			mutexPool = pool;
		}

		platform::MutexPool& getEffectiveMutexPool() const;

		inline Stat::UserID getCurrentUser() const {
			return currentUser;
		}

		inline void setCurrentUser(Stat::UserID user) {
			currentUser = user;
		}

		inline Stat::GroupID getCurrentGroup() const {
			return currentGroup;
		}

		inline void setCurrentGroup(Stat::GroupID group) {
			currentGroup = group;
		}

		inline int getBaseFlags() const {
			return flags;
		}

		inline void setBaseFlags(int flags) {
			this->flags = flags;
		}

		inline bool hasBaseFlag(int mask) {
			return (flags & mask) == mask;
		}

		inline void setBaseFlag(int mask) {
			flags |= mask;
		}

		inline void unsetBaseFlag(int mask) {
			flags &= ~mask;
		}

		inline int getDefaultFilePermissions() const {
			return defaultPermissions;
		}

		inline void setDefaultFilePermissions(int permissions) {
			defaultPermissions = permissions;
		}

		virtual MemoryDirectory* createDirectory(int);
		virtual MemoryFile* createSymlink(const text::String16&);
		virtual MemoryFile* createDeviceFile(int, bool, Stat::DeviceID);
		virtual MemoryFile* createRegularFile(int) = 0;

		virtual size_t getDeviceSize(bool, Stat::DeviceID);
		virtual void truncateDevice(bool, Stat::DeviceID, size_t);
		virtual io::InputStream<char>* getDeviceInputStream(bool, Stat::DeviceID);
		virtual io::OutputStream<char>* getDeviceOutputStream(bool, Stat::DeviceID);
		virtual io::BidirectionalStream<char>* getDeviceStream(bool, Stat::DeviceID, bool);

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
		virtual void truncate(PathIterator, PathIterator, size_t);
		virtual void statfs(PathIterator, PathIterator, FSInfo&);
		virtual void mknod(PathIterator, PathIterator, Stat::Type, int, Stat::DeviceID);
		virtual io::InputStream<char>* getInputStream(PathIterator, PathIterator);
		virtual io::OutputStream<char>* getOutputStream(PathIterator, PathIterator);
		virtual io::BidirectionalStream<char>* getStream(PathIterator, PathIterator, bool);
		virtual VFile* getFileReference(PathIterator, PathIterator, bool);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_MEMORYVFSBASE_HPP */

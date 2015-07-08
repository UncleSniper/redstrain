#ifndef REDSTRAIN_MOD_VFS_MEMORYVFSBASE_HPP
#define REDSTRAIN_MOD_VFS_MEMORYVFSBASE_HPP

#include <map>
#include <redstrain/platform/MutexPool.hpp>
#include <redstrain/util/ReferenceCounted.hpp>

#include "VFS.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API MemoryVFSBase : public VFS {

	  public:
		class REDSTRAIN_VFS_API MemoryFile : private util::ReferenceCounted {

		  private:
			MemoryVFSBase* fs;
			Stat::UserID owner;
			Stat::GroupID group;
			int permissions;
			time_t atime, mtime, ctime;

		  public:
			MemoryFile(MemoryVFSBase*);
			MemoryFile(const MemoryFile&);
			virtual ~MemoryFile();

			inline MemoryVFSBase* getMemoryVFSBase() const {
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

			virtual Stat::Type getFileType() const = 0;
			virtual void stat(Stat&, bool) = 0;
			virtual void chmod(int);
			virtual void chown(Stat::UserID, bool);
			virtual void chgrp(Stat::GroupID, bool);
			virtual void utime();
			virtual void utime(time_t, time_t);
			virtual bool access(int);
			virtual void rmdir();
			virtual void readlink(text::String16&);
			virtual void truncate(size_t);
			virtual io::InputStream<char>* getInputStream();
			virtual io::OutputStream<char>* getOutputStream();
			virtual io::BidirectionalStream<char>* getStream(bool);

		};

		class REDSTRAIN_VFS_API MemoryDirectory : public MemoryFile {

		  public:
			MemoryDirectory(MemoryVFSBase*);
			MemoryDirectory(const MemoryDirectory&);

			virtual Stat::Type getFileType() const;
			virtual void readdir(util::Appender<text::String16>&) = 0;
			virtual MemoryFile* getEntry(const text::String16&) = 0;
			virtual void putEntry(const text::String16&) = 0;

		};

		class REDSTRAIN_VFS_API SimpleMemoryDirectory : public MemoryDirectory {

		  private:
			typedef std::map<text::String16, MemoryFile*> EntryMap;

		  private:
			EntryMap entries;

		  public:
			SimpleMemoryDirectory(MemoryVFSBase*);
			SimpleMemoryDirectory(const SimpleMemoryDirectory&);
			virtual ~SimpleMemoryDirectory();

			virtual void readdir(util::Appender<text::String16>&);
			virtual MemoryFile* getEntry(const text::String16&);
			virtual void putEntry(const text::String16&);

		};

	  private:
		MemoryDirectory* root;
		platform::MutexPool* mutexPool;

	  public:
		MemoryVFSBase();
		MemoryVFSBase(const MemoryVFSBase&);
		virtual ~MemoryVFSBase();

		inline platform::MutexPool* getMutexPool() const {
			return mutexPool;
		}

		inline void setMutexPool(platform::MutexPool* pool) {
			mutexPool = pool;
		}

		platform::MutexPool& getEffectiveMutexPool() const;

		virtual MemoryDirectory* createDirectory() = 0;

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
		virtual VFile* getFileReference(PathIterator, PathIterator);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_MEMORYVFSBASE_HPP */

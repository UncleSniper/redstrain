#ifndef REDSTRAIN_MOD_VFS_BLOBVFS_HPP
#define REDSTRAIN_MOD_VFS_BLOBVFS_HPP

#include <set>
#include <string>
#include <redstrain/util/ReferenceCounted.hpp>

#include "MemoryBase.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API BlobVFS : public MemoryBase {

	  public:
		class REDSTRAIN_VFS_API BlobMemoryFile : public MemoryFile {

		  private:
			const char* data;
			size_t size;

		  public:
			BlobMemoryFile(MemoryBase&, int, const char*, size_t);
			BlobMemoryFile(const BlobMemoryFile&);

			inline const char* getData() const {
				return data;
			}

			inline size_t getSize() const {
				return size;
			}

			virtual Stat::Type getFileType() const;
			virtual void stat(Stat&);
			virtual void truncate(size_t);
			virtual io::InputStream<char>* getInputStream();
			virtual io::OutputStream<char>* getOutputStream();
			virtual io::BidirectionalStream<char>* getStream(bool);

		};

		class REDSTRAIN_VFS_API BlobEmitter : public util::ReferenceCounted {

		  public:
			BlobEmitter();
			BlobEmitter(const BlobEmitter&);

			virtual void emitBlobs(BlobVFS&) = 0;

		};

		class REDSTRAIN_VFS_API BlobInjector : public BlobEmitter {

		  private:
			const char *const data;
			const size_t size;
			const std::string path;

		  public:
			BlobInjector(const char*, size_t, const std::string&);
			BlobInjector(const BlobInjector&);

			inline const char* getData() const {
				return data;
			}

			inline size_t getSize() const {
				return size;
			}

			inline const std::string& getPath() const {
				return path;
			}

			virtual void emitBlobs(BlobVFS&);

		};

		class REDSTRAIN_VFS_API BlobLinker : public BlobEmitter {

		  private:
			const std::string oldPath, newPath;

		  public:
			BlobLinker(const std::string&, const std::string&);
			BlobLinker(const BlobLinker&);

			inline const std::string& getOldPath() const {
				return oldPath;
			}

			inline const std::string& getNewPath() const {
				return newPath;
			}

			virtual void emitBlobs(BlobVFS&);

			static void generateLinkers(io::InputStream<char>&, io::OutputStream<char>&,
					const std::string& = "", const std::string& = "");

		};

	  private:
		typedef std::set<BlobEmitter*> BlobEmitters;

	  private:
		static BlobEmitters emitters;

	  public:
		static const int DEFAULT_BASE_FLAGS = MemoryBase::BFL_READONLY;
		static const int DEFAULT_DIRECTORY_PERMISSIONS = 0755;

	  private:
		int defaultDirectoryPermissions;

	  protected:
		BlobVFS(MemoryDirectory*, int);
		BlobVFS(const BlobVFS&);

	  public:
		BlobVFS(int = DEFAULT_BASE_FLAGS);

		inline int getDefaultDirectoryPermissions() const {
			return defaultDirectoryPermissions;
		}

		inline void setDefaultDirectoryPermissions(int permissions) {
			defaultDirectoryPermissions = permissions;
		}

		void putBlob(const std::string&, const char*, size_t);
		void putBlob(const text::String16&, const char*, size_t);
		void putBlob(const Pathname&, const char*, size_t);
		void aliasBlob(const std::string&, const std::string&);
		void aliasBlob(const text::String16&, const text::String16&);
		void aliasBlob(const text::String16&, const Pathname&);
		void putEmittedBlobs();

		virtual MemoryFile* createRegularFile(int);

		static void addBlobEmitter(BlobEmitter*);
		static void removeBlobEmitter(BlobEmitter*);
		static BlobVFS& getDefaultBlobVFS();

	};

}}

#endif /* REDSTRAIN_MOD_VFS_BLOBVFS_HPP */

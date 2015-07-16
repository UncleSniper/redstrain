#ifndef REDSTRAIN_MOD_VFS_BLOBVFS_HPP
#define REDSTRAIN_MOD_VFS_BLOBVFS_HPP

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

	  public:
		static const int DEFAULT_BASE_FLAGS = MemoryBase::BFL_READONLY;

	  protected:
		BlobVFS(MemoryDirectory*, int);
		BlobVFS(const BlobVFS&);

	  public:
		BlobVFS(int = DEFAULT_BASE_FLAGS);

		void putBlob(const std::string&, const char*, size_t);
		void putBlob(const text::String16&, const char*, size_t);
		void putBlob(const Pathname&, const char*, size_t);

		virtual MemoryFile* createRegularFile(int);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_BLOBVFS_HPP */

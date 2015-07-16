#ifndef REDSTRAIN_MOD_VFS_MEMORYVFS_HPP
#define REDSTRAIN_MOD_VFS_MEMORYVFS_HPP

#include <redstrain/algorithm/Rope.hpp>

#include "MemoryBase.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API MemoryVFS : public MemoryBase {

	  public:
		class REDSTRAIN_VFS_API RopeMemoryFile : public MemoryFile {

		  private:
			algorithm::Rope<char> content;

		  public:
			RopeMemoryFile(MemoryBase&, int);
			RopeMemoryFile(const RopeMemoryFile&);

			inline algorithm::Rope<char>& getContent() {
				return content;
			}

			inline const algorithm::Rope<char>& getContent() const {
				return content;
			}

			virtual Stat::Type getFileType() const;
			virtual void stat(Stat&);
			virtual void truncate(size_t);
			virtual io::InputStream<char>* getInputStream();
			virtual io::OutputStream<char>* getOutputStream();
			virtual io::BidirectionalStream<char>* getStream(bool);

		};

		class REDSTRAIN_VFS_API RopeBase : public virtual io::Stream {

		  private:
			bool closed;

		  protected:
			RopeMemoryFile& file;
			size_t position;

		  protected:
			RopeBase(const RopeBase&);

		  public:
			RopeBase(RopeMemoryFile&, bool);
			virtual ~RopeBase();

			inline RopeMemoryFile& getFile() {
				return file;
			}

			inline const RopeMemoryFile& getFile() const {
				return file;
			}

			inline bool isClosed() const {
				return closed;
			}

			virtual void close();
			virtual void seek(off_t, SeekWhence);
			virtual size_t tell() const;

		};

		class REDSTRAIN_VFS_API RopeInputStream : public virtual RopeBase, public virtual io::InputStream<char> {

		  protected:
			RopeInputStream(const RopeInputStream&);

			virtual size_t readBlock(char*, size_t);

		  public:
			RopeInputStream(RopeMemoryFile&, bool = false);

			virtual void seek(off_t, SeekWhence);

		};

		class REDSTRAIN_VFS_API RopeOutputStream : public virtual RopeBase, public virtual io::OutputStream<char> {

		  protected:
			RopeOutputStream(const RopeOutputStream&);

			virtual void writeBlock(const char*, size_t);

		  public:
			RopeOutputStream(RopeMemoryFile&, bool = true);

		};

		class REDSTRAIN_VFS_API RopeStream : public virtual RopeInputStream, public virtual RopeOutputStream,
				public virtual io::BidirectionalStream<char> {

		  protected:
			RopeStream(const RopeStream&);

		  public:
			RopeStream(RopeMemoryFile&, bool = false);

		};

	  public:
		static const int DEFAULT_BASE_FLAGS = 0;

	  protected:
		MemoryVFS(MemoryDirectory*, int);
		MemoryVFS(const MemoryVFS&);

	  public:
		MemoryVFS(int = DEFAULT_BASE_FLAGS);

		virtual MemoryFile* createRegularFile(int);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_MEMORYVFS_HPP */

#ifndef REDSTRAIN_MOD_VFS_VFS_HPP
#define REDSTRAIN_MOD_VFS_VFS_HPP

#include <list>
#include <redstrain/text/Encoder16.hpp>
#include <redstrain/text/Decoder16.hpp>
#include <redstrain/text/CodecFactory.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/io/BidirectionalStream.hpp>

#include "Stat.hpp"

namespace redengine {
namespace vfs {

	class VFile;

	class REDSTRAIN_VFS_API VFS {

	  public:
		typedef std::list<text::String16> Pathname;
		typedef Pathname::const_iterator PathIterator;

		enum {
			FILE_EXISTS = 00,
			CAN_READ    = 04,
			CAN_WRITE   = 02,
			CAN_EXECUTE = 01
		};

		enum {
			REMOVE_SOURCE        = 001,
			PRESERVE_OWNER       = 002,
			PRESERVE_GROUP       = 004,
			PRESERVE_PERMISSIONS = 010,
			PRESERVE_TIMESTAMPS  = 020,
			PRESERVE_ALL         = 036
		};

		enum FSType {
			UNKNOWNFS,
			ADFS,
			AFFS,
			BEFS,
			BFS,
			CIFS,
			CODA,
			COH,
			CRAMFS,
			DEVFS,
			EFS,
			EXT,
			EXT2,
			EXT3,
			HFS,
			HPFS,
			HUGETLBFS,
			ISOFS,
			JFFS2,
			JFS,
			MINIX,
			MINIX2,
			MSDOS,
			NCP,
			NFS,
			NTFS,
			OPENPROM,
			PROC,
			QNX4,
			REISERFS,
			ROMFS,
			SMB,
			SYSV2,
			SYSV4,
			TMPFS,
			UDF,
			UFS,
			USBDEV,
			VXFS,
			XENIX,
			XFS,
			XIAFS
		};

		class FSInfo {

		  private:
			FSType type;
			size_t totalBlocks, freeBlocks, totalINodes, freeINodes, nameLength;

		  public:
			FSInfo();
			FSInfo(const FSInfo&);

			inline FSType getType() const {
				return type;
			}

			inline void setType(FSType newType) {
				type = newType;
			}

			inline size_t getTotalBlockCount() const {
				return totalBlocks;
			}

			inline void setTotalBlockCount(size_t newBlockCount) {
				totalBlocks = newBlockCount;
			}

			inline size_t getFreeBlockCount() const {
				return freeBlocks;
			}

			inline void setFreeBlockCount(size_t newBlockCount) {
				freeBlocks = newBlockCount;
			}

			inline size_t getTotalINodeCount() const {
				return totalINodes;
			}

			inline void setTotalINodeCount(size_t newINodeCount) {
				totalINodes = newINodeCount;
			}

			inline size_t getFreeINodeCount() const {
				return freeINodes;
			}

			inline void setFreeINodeCount(size_t newINodeCount) {
				freeINodes = newINodeCount;
			}

			inline size_t getMaximumFilenameLength() const {
				return nameLength;
			}

			inline void setMaximumFilenameLength(size_t newMaxLength) {
				nameLength = newMaxLength;
			}

			FSInfo& operator=(const FSInfo&);

		};

		class EncodingAppender : public util::Appender<text::String16> {

		  private:
			util::Appender<std::string>& sink;
			VFS& encoder;

		  public:
			EncodingAppender(util::Appender<std::string>&, VFS&);

			inline util::Appender<std::string>& getSink() {
				return sink;
			}

			inline const util::Appender<std::string>& getSink() const {
				return sink;
			}

			inline VFS& getEncoder() {
				return encoder;
			}

			inline const VFS& getEncoder() const {
				return encoder;
			}

			virtual void append(const text::String16&);
			virtual void doneAppending();

		};

	  public:
		static const text::String16 SELF_LINK;
		static const text::String16 PARENT_LINK;

	  private:
		text::CodecFactory<text::Encoder16>* encoderFactory;
		text::CodecFactory<text::Decoder16>* decoderFactory;

	  private:
		static void pushPathSegment(Pathname&, const text::String16&);

	  protected:
		VFS(const VFS&);

	  public:
		VFS();
		virtual ~VFS();

		inline text::CodecFactory<text::Encoder16>* getPathnameEncoderFactory() const {
			return encoderFactory;
		}

		void setPathnameEncoderFactory(text::CodecFactory<text::Encoder16>*);

		inline text::CodecFactory<text::Decoder16>* getPathnameDecoderFactory() const {
			return decoderFactory;
		}

		void setPathnameDecoderFactory(text::CodecFactory<text::Decoder16>*);

		text::String16 decodePathname(const std::string&) const;
		std::string encodePathname(const text::String16&) const;
		void deconstructPathname(const std::string&, Pathname&) const;
		std::string constructBytePathname(const Pathname&, bool) const;
		std::string constructBytePathname(PathIterator, PathIterator, bool) const;

		void stat(const std::string&, Stat&, bool);
		void stat(const text::String16&, Stat&, bool);
		void stat(const Pathname&, Stat&, bool);
		void chmod(const std::string&, int);
		void chmod(const text::String16&, int);
		void chmod(const Pathname&, int);
		void chown(const std::string&, Stat::UserID, bool);
		void chown(const text::String16&, Stat::UserID, bool);
		void chown(const Pathname&, Stat::UserID, bool);
		void chgrp(const std::string&, Stat::GroupID, bool);
		void chgrp(const text::String16&, Stat::GroupID, bool);
		void chgrp(const Pathname&, Stat::GroupID, bool);
		void link(const std::string&, const std::string&);
		void link(const text::String16&, const text::String16&);
		void link(const Pathname&, const Pathname&);
		void unlink(const std::string&);
		void unlink(const text::String16&);
		void unlink(const Pathname&);
		void utime(const std::string&);
		void utime(const text::String16&);
		void utime(const Pathname&);
		void utime(const std::string&, time_t, time_t);
		void utime(const text::String16&, time_t, time_t);
		void utime(const Pathname&, time_t, time_t);
		bool access(const std::string&, int);
		bool access(const text::String16&, int);
		bool access(const Pathname&, int);
		void rename(const std::string&, const std::string&);
		void rename(const text::String16&, const text::String16&);
		void rename(const Pathname&, const Pathname&);
		void mkdir(const std::string&, int);
		void mkdir(const text::String16&, int);
		void mkdir(const Pathname&, int);
		void rmdir(const std::string&);
		void rmdir(const text::String16&);
		void rmdir(const Pathname&);
		void symlink(const std::string&, const std::string&);
		void symlink(const text::String16&, const text::String16&);
		void symlink(const text::String16&, const Pathname&);
		void readlink(const std::string&, std::string&);
		void readlink(const text::String16&, text::String16&);
		void readlink(const Pathname&, text::String16&);
		void readdir(const std::string&, util::Appender<std::string>&);
		void readdir(const text::String16&, util::Appender<text::String16>&);
		void readdir(const Pathname&, util::Appender<text::String16>&);
		void truncate(const std::string&, size_t);
		void truncate(const text::String16&, size_t);
		void truncate(const Pathname&, size_t);
		void statfs(const std::string&, FSInfo&);
		void statfs(const text::String16&, FSInfo&);
		void statfs(const Pathname&, FSInfo&);
		void mknod(const std::string&, Stat::Type, int, Stat::DeviceID);
		void mknod(const text::String16&, Stat::Type, int, Stat::DeviceID);
		void mknod(const Pathname&, Stat::Type, int, Stat::DeviceID);
		io::InputStream<char>* getInputStream(const std::string&);
		io::InputStream<char>* getInputStream(const text::String16&);
		io::InputStream<char>* getInputStream(const Pathname&);
		io::OutputStream<char>* getOutputStream(const std::string&);
		io::OutputStream<char>* getOutputStream(const text::String16&);
		io::OutputStream<char>* getOutputStream(const Pathname&);
		io::BidirectionalStream<char>* getStream(const std::string&, bool);
		io::BidirectionalStream<char>* getStream(const text::String16&, bool);
		io::BidirectionalStream<char>* getStream(const Pathname&, bool);
		VFile* getFileReference(const std::string&);
		VFile* getFileReference(const text::String16&);
		VFile* getFileReference(const Pathname&);

		void removeRecursively(const std::string&);
		void removeRecursively(const text::String16&);
		void removeRecursively(const Pathname&);
		void removeRecursively(PathIterator, PathIterator);

		void copyTree(const std::string&, VFS&, const std::string&, int = PRESERVE_ALL);
		void copyTree(const text::String16&, VFS&, const text::String16&, int = PRESERVE_ALL);
		void copyTree(const Pathname&, VFS&, const Pathname&, int = PRESERVE_ALL);
		void copyTree(PathIterator, PathIterator, VFS&, PathIterator, PathIterator, int = PRESERVE_ALL);
		void copyTree(const std::string&, const std::string&, int = PRESERVE_ALL);
		void copyTree(const text::String16&, const text::String16&, int = PRESERVE_ALL);
		void copyTree(const Pathname&, const Pathname&, int = PRESERVE_ALL);
		void copyTree(PathIterator, PathIterator, PathIterator, PathIterator, int = PRESERVE_ALL);

		virtual void stat(PathIterator, PathIterator, Stat&, bool) = 0;
		virtual void chmod(PathIterator, PathIterator, int) = 0;
		virtual void chown(PathIterator, PathIterator, Stat::UserID, bool) = 0;
		virtual void chgrp(PathIterator, PathIterator, Stat::GroupID, bool) = 0;
		virtual void link(PathIterator, PathIterator, PathIterator, PathIterator) = 0;
		virtual void unlink(PathIterator, PathIterator) = 0;
		virtual void utime(PathIterator, PathIterator) = 0;
		virtual void utime(PathIterator, PathIterator, time_t, time_t) = 0;
		virtual bool access(PathIterator, PathIterator, int) = 0;
		virtual void sync() = 0;
		virtual void rename(PathIterator, PathIterator, PathIterator, PathIterator) = 0;
		virtual void mkdir(PathIterator, PathIterator, int) = 0;
		virtual void rmdir(PathIterator, PathIterator) = 0;
		virtual void symlink(const text::String16&, PathIterator, PathIterator) = 0;
		virtual void readlink(PathIterator, PathIterator, text::String16&) = 0;
		virtual void readdir(PathIterator, PathIterator, util::Appender<text::String16>&) = 0;
		virtual void truncate(PathIterator, PathIterator, size_t) = 0;
		virtual void statfs(PathIterator, PathIterator, FSInfo&) = 0;
		virtual void mknod(PathIterator, PathIterator, Stat::Type, int, Stat::DeviceID) = 0;
		virtual io::InputStream<char>* getInputStream(PathIterator, PathIterator) = 0;
		virtual io::OutputStream<char>* getOutputStream(PathIterator, PathIterator) = 0;
		virtual io::BidirectionalStream<char>* getStream(PathIterator, PathIterator, bool) = 0;
		virtual VFile* getFileReference(PathIterator, PathIterator);

		static void deconstructPathname(const text::String16&, Pathname&);
		static text::String16 constructPathname(const Pathname&, bool);
		static text::String16 constructPathname(PathIterator, PathIterator, bool);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_VFS_HPP */

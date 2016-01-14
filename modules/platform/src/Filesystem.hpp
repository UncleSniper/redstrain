#ifndef REDSTRAIN_MOD_PLATFORM_FILESYSTEM_HPP
#define REDSTRAIN_MOD_PLATFORM_FILESYSTEM_HPP

#include <string>
#include <redstrain/util/Appender.hpp>

#include "Stat.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Filesystem {

	  public:
		enum {
			FILE_EXISTS = 00,
			CAN_READ    = 04,
			CAN_WRITE   = 02,
			CAN_EXECUTE = 01
		};

		enum REDSTRAIN_PLATFORM_API FSType {
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

		class REDSTRAIN_PLATFORM_API FSInfo {

		  private:
			FSType type;
			util::FileSize totalBlocks, freeBlocks, totalINodes, freeINodes, nameLength;

		  public:
			FSInfo();
			FSInfo(const FSInfo&);

			inline FSType getType() const {
				return type;
			}

			inline void setType(FSType newType) {
				type = newType;
			}

			inline util::FileSize getTotalBlockCount() const {
				return totalBlocks;
			}

			inline void setTotalBlockCount(util::FileSize newBlockCount) {
				totalBlocks = newBlockCount;
			}

			inline util::FileSize getFreeBlockCount() const {
				return freeBlocks;
			}

			inline void setFreeBlockCount(util::FileSize newBlockCount) {
				freeBlocks = newBlockCount;
			}

			inline util::FileSize getTotalINodeCount() const {
				return totalINodes;
			}

			inline void setTotalINodeCount(util::FileSize newINodeCount) {
				totalINodes = newINodeCount;
			}

			inline util::FileSize getFreeINodeCount() const {
				return freeINodes;
			}

			inline void setFreeINodeCount(util::FileSize newINodeCount) {
				freeINodes = newINodeCount;
			}

			inline util::FileSize getMaximumFilenameLength() const {
				return nameLength;
			}

			inline void setMaximumFilenameLength(util::FileSize newMaxLength) {
				nameLength = newMaxLength;
			}

			FSInfo& operator=(const FSInfo&);

		};

		class REDSTRAIN_PLATFORM_API TraversalSink {

		  public:
			TraversalSink();
			TraversalSink(const TraversalSink&);
			virtual ~TraversalSink();

			virtual bool enterDirectory(const std::string&) = 0;
			virtual void visitFile(const std::string&) = 0;
			virtual void leaveDirectory(const std::string&, bool) = 0;

		};

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef int ErrorCode;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		typedef DWORD ErrorCode;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
	  private:
		static ErrorCode movePreserving(const std::string&, const std::string&);
		static ErrorCode moveFilePreserving(const std::string&, const std::string&);
		static ErrorCode moveDirectoryPreserving(const std::string&, const std::string&);
#endif /* OS-specific helper functions */

	  public:
		static void stat(const std::string&, Stat&, bool = false);
		static void chmod(const std::string&, int);
		static void chown(const std::string&, Stat::UserID, bool = false);
		static void chgrp(const std::string&, Stat::GroupID, bool = false);
		static void link(const std::string&, const std::string&);
		static void unlink(const std::string&);
		static void utime(const std::string&);
		static void utime(const std::string&, time_t, time_t);
		static bool access(const std::string&, int);
		static void sync();
		static void rename(const std::string&, const std::string&);
		static void mkdir(const std::string&, int = Stat::DEFAULT_DIRECTORY_PERMISSIONS);
		static void rmdir(const std::string&);
		static void symlink(const std::string&, const std::string&);
		static void readlink(const std::string&, std::string&);
		static void readdir(const std::string&, util::Appender<std::string>&);
		static void truncate(const std::string&, util::FileSize);
		static void statfs(const std::string&, FSInfo&);
		static void listRoots(util::Appender<std::string>&);
		static void mknod(const std::string&, Stat::Type, int, Stat::DeviceID = Stat::NO_DEVICE);

		static std::string getErrorMessage(ErrorCode);
		static Stat::UserID getCurrentUser();
		static Stat::GroupID getCurrentGroup();
		static void removeRecursively(const std::string&);
		static void traverse(const std::string&, TraversalSink&);
		static void mkdirRecursive(const std::string&);
		static std::string getSystemTempDirectory();
		static bool mkdirExclusive(const std::string&, int, bool);
		static bool creatExclusive(const std::string&, int, bool);
		static std::string makeTempDirectory(const std::string& = "", int = 0700);
		static std::string makeTempFile(const std::string& = "", int = 0600);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FILESYSTEM_HPP */

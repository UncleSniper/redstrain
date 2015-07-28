#include <redstrain/util/DeleteArray.hpp>
#include <redstrain/util/IntegerBits.hpp>

#include "Thread.hpp"
#include "Pathname.hpp"
#include "NoSuchFileError.hpp"
#include "FileAccessDeniedError.hpp"
#include "ReadOnlyFilesystemError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <utime.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/statfs.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <Sddl.h>
#include <Aclapi.h>
#include <AccCtrl.h>
#endif /* OS-specific includes */

using std::string;
using redengine::util::Appender;
using redengine::util::DeleteArray;
using redengine::util::IntegerBits;

namespace redengine {
namespace platform {

	// ======== FSInfo ========

	Filesystem::FSInfo::FSInfo() : type(UNKNOWNFS), totalBlocks(static_cast<size_t>(0u)),
			freeBlocks(static_cast<size_t>(0u)), totalINodes(static_cast<size_t>(0u)),
			freeINodes(static_cast<size_t>(0u)), nameLength(static_cast<size_t>(0u)) {}

	Filesystem::FSInfo::FSInfo(const FSInfo& info) : type(info.type), totalBlocks(info.totalBlocks),
			freeBlocks(info.freeBlocks), totalINodes(info.totalINodes), freeINodes(info.freeINodes),
			nameLength(info.nameLength) {}

	Filesystem::FSInfo& Filesystem::FSInfo::operator=(const FSInfo& info) {
		type = info.type;
		totalBlocks = info.totalBlocks;
		freeBlocks = info.freeBlocks;
		totalINodes = info.totalINodes;
		freeINodes = info.freeINodes;
		nameLength = info.nameLength;
		return *this;
	}

	// ======== TraversalSink ========

	Filesystem::TraversalSink::TraversalSink() {}

	Filesystem::TraversalSink::TraversalSink(const TraversalSink&) {}

	Filesystem::TraversalSink::~TraversalSink() {}

	// ======== Filesystem ========

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	static void bail() {
		switch(errno) {
			case ENOENT:
				throw NoSuchFileError(errno);
			case EACCES:
				throw FileAccessDeniedError(errno);
			case EROFS:
				throw ReadOnlyFilesystemError(errno);
			default:
				throw FilesystemError(errno);
		}
	}

	void Filesystem::stat(const string& path, Stat& info, bool ofLink) {
		struct stat native;
		if((ofLink ? ::lstat : ::stat)(path.c_str(), &native))
			bail();
		switch(native.st_mode & S_IFMT) {
			case S_IFDIR:
				info.setType(Stat::DIRECTORY);
				break;
			case S_IFCHR:
				info.setType(Stat::CHARACTER_DEVICE);
				break;
			case S_IFBLK:
				info.setType(Stat::BLOCK_DEVICE);
				break;
			case S_IFIFO:
				info.setType(Stat::NAMED_PIPE);
				break;
			case S_IFLNK:
				info.setType(Stat::SYMBOLIC_LINK);
				break;
			case S_IFSOCK:
				info.setType(Stat::LOCAL_SOCKET);
				break;
			default:
				info.setType(Stat::REGULAR_FILE);
				break;
		}
		info.setOwner(native.st_uid);
		info.setGroup(native.st_gid);
		info.setDevice(native.st_dev);
		info.setSpecialSpecifier(native.st_rdev);
		info.setPermissions(native.st_mode & 0777);
		info.setSize(static_cast<size_t>(native.st_size));
		info.setAccessTimestamp(native.st_atime);
		info.setModificationTimestamp(native.st_mtime);
		info.setStatusChangeTimestamp(native.st_ctime);
	}

	void Filesystem::chmod(const string& path, int permissions) {
		if(::chmod(path.c_str(), static_cast<mode_t>(permissions & 0777)))
			bail();
	}

	void Filesystem::chown(const string& path, Stat::UserID owner, bool ofLink) {
		if((ofLink ? ::lchown : ::chown)(path.c_str(), owner, static_cast<gid_t>(-1)))
			bail();
	}

	void Filesystem::chgrp(const string& path, Stat::GroupID group, bool ofLink) {
		if((ofLink ? ::lchown : ::chown)(path.c_str(), static_cast<uid_t>(-1), group))
			bail();
	}

	void Filesystem::link(const string& oldPath, const string& newPath) {
		if(::link(oldPath.c_str(), newPath.c_str()))
			bail();
	}

	void Filesystem::unlink(const string& path) {
		if(::unlink(path.c_str()))
			bail();
	}

	void Filesystem::utime(const string& path) {
		if(::utime(path.c_str(), NULL))
			bail();
	}

	void Filesystem::utime(const string& path, time_t accessTimestamp, time_t modificationTimestamp) {
		struct utimbuf buffer;
		buffer.actime = accessTimestamp;
		buffer.modtime = modificationTimestamp;
		if(::utime(path.c_str(), &buffer))
			bail();
	}

	bool Filesystem::access(const string& path, int permissions) {
		int mode = 0;
		if(permissions == FILE_EXISTS)
			mode = F_OK;
		else {
			if(permissions & CAN_READ)
				mode |= R_OK;
			if(permissions & CAN_WRITE)
				mode |= W_OK;
			if(permissions & CAN_EXECUTE)
				mode |= X_OK;
		}
		if(!::access(path.c_str(), mode))
			return true;
		if(errno != (permissions == FILE_EXISTS ? ENOENT : EACCES))
			bail();
		return false;
	}

	void Filesystem::sync() {
		::sync();
	}

	static int clearMoveDestination(const string& newPath) {
		struct stat ninfo;
		bool newExists = !::lstat(newPath.c_str(), &ninfo);
		if(newExists) {
			if((S_ISDIR(ninfo.st_mode)) ? ::rmdir(newPath.c_str()) : ::unlink(newPath.c_str()))
				return errno;
		}
		else if(errno != ENOENT)
			return errno;
		return 0;
	}

	int Filesystem::movePreserving(const string& oldPath, const string& newPath) {
		struct stat info;
		if(::lstat(oldPath.c_str(), &info))
			return errno;
		int error;
		switch(info.st_mode & S_IFMT) {
			case S_IFDIR:
				error = moveDirectoryPreserving(oldPath, newPath);
				break;
			case S_IFCHR:
			case S_IFBLK:
			case S_IFIFO:
			case S_IFSOCK:
				{
					error = clearMoveDestination(newPath);
					if(error)
						return error;
					if(::mknod(newPath.c_str(), info.st_mode & (S_IFMT | 0777), info.st_rdev))
						error = errno;
				}
				break;
			case S_IFLNK:
				{
					error = clearMoveDestination(newPath);
					if(error)
						return error;
					string target;
					readlink(oldPath, target);
					if(::symlink(target.c_str(), newPath.c_str()))
						return errno;
				}
				break;
			default:
				error = moveFilePreserving(oldPath, newPath);
				break;
		}
		if(error)
			return error;
		::lchown(newPath.c_str(), info.st_uid, info.st_gid);
		if(!(S_ISLNK(info.st_mode))) {
			::chmod(newPath.c_str(), static_cast<mode_t>(info.st_mode & 0777));
			struct utimbuf times;
			times.actime = info.st_atime;
			times.modtime = info.st_mtime;
			::utime(newPath.c_str(), &times);
		}
		return 0;
	}

	int Filesystem::moveFilePreserving(const string& oldPath, const string& newPath) {
		int error = clearMoveDestination(newPath);
		if(error)
			return error;
		int in = open(oldPath.c_str(), O_RDONLY);
		if(in == -1)
			return errno;
		int out = open(newPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, static_cast<mode_t>(0400));
		if(out == -1) {
			close(in);
			return errno;
		}
		char buffer[512];
		for(;;) {
			ssize_t count = read(in, buffer, sizeof(buffer));
			if(count == static_cast<ssize_t>(-1)) {
				close(in);
				close(out);
				unlink(newPath.c_str());
				return errno;
			}
			if(!count)
				break;
			ssize_t written = static_cast<ssize_t>(0), writtenThisIteration;
			while(written < count) {
				writtenThisIteration = write(out, buffer + written, static_cast<size_t>(count - written));
				if(writtenThisIteration == static_cast<ssize_t>(-1)) {
					close(in);
					close(out);
					unlink(newPath.c_str());
					return errno;
				}
				written += writtenThisIteration;
			}
		}
		if(close(in)) {
			close(out);
			return errno;
		}
		if(close(out))
			return errno;
		return 0;
	}

	int Filesystem::moveDirectoryPreserving(const string& oldPath, const string& newPath) {
		struct stat ninfo;
		bool newExists = !::stat(newPath.c_str(), &ninfo);
		bool created = false;
		if(newExists) {
			if(!(S_ISDIR(ninfo.st_mode))) {
				if(::unlink(newPath.c_str()))
					return errno;
				if(::mkdir(newPath.c_str(), static_cast<mode_t>(0700)))
					return errno;
				created = true;
			}
		}
		else {
			if(errno != ENOENT)
				return errno;
			if(::mkdir(newPath.c_str(), static_cast<mode_t>(0700)))
				return errno;
			created = true;
		}
		DIR* dir = opendir(oldPath.c_str());
		if(!dir) {
			if(created)
				::rmdir(newPath.c_str());
			return errno;
		}
		struct dirent* ent;
		string src(oldPath), dest(newPath);
		if(src.empty() || src[src.length() - static_cast<string::size_type>(1u)] != '/')
			src += '/';
		if(dest.empty() || dest[dest.length() - static_cast<string::size_type>(1u)] != '/')
			dest += '/';
		bool stillEmpty = true;
		while((ent = ::readdir(dir))) {
			if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
				continue;
			int error = movePreserving(src + ent->d_name, dest + ent->d_name);
			if(error) {
				closedir(dir);
				if(created && stillEmpty)
					::rmdir(newPath.c_str());
				return error;
			}
			stillEmpty = false;
		}
		if(closedir(dir))
			return errno;
		return 0;
	}

	void Filesystem::rename(const string& oldPath, const string& newPath) {
		if(!::rename(oldPath.c_str(), newPath.c_str()))
			return;
		if(errno != EXDEV)
			bail();
		// Now we are, of course, seriously screwed.
		// We need to do the whole damn thing by hand:
		int error = movePreserving(oldPath, newPath);
		if(error) {
			errno = error;
			bail();
		}
	}

	void Filesystem::mkdir(const string& path, int permissions) {
		if(::mkdir(path.c_str(), static_cast<mode_t>(permissions & 0777)))
			bail();
	}

	void Filesystem::rmdir(const string& path) {
		if(::rmdir(path.c_str()))
			bail();
	}

	void Filesystem::symlink(const string& oldPath, const string& newPath) {
		if(::symlink(oldPath.c_str(), newPath.c_str()))
			bail();
	}

	void Filesystem::readlink(const string& path, string& result) {
		struct stat info;
		if(::stat(path.c_str(), &info))
			bail();
		size_t bufsize = info.st_size ? info.st_size + static_cast<size_t>(1u) : static_cast<size_t>(256u);
		DeleteArray<char> buffer;
		for(;;) {
			buffer = new char[bufsize];
			ssize_t count = ::readlink(path.c_str(), *buffer, bufsize);
			if(count == static_cast<ssize_t>(-1))
				bail();
			if(static_cast<size_t>(count) < bufsize) {
				result.assign(*buffer, static_cast<string::size_type>(count));
				return;
			}
			size_t nextSize = bufsize * static_cast<size_t>(2u);
			if(nextSize < bufsize) {
				nextSize = bufsize + static_cast<size_t>(255u);
				if(nextSize < bufsize) {
					result.assign(*buffer, static_cast<string::size_type>(count));
					return;
				}
			}
			delete[] buffer.set();
		}
	}

	struct DirectoryCloser {

		DIR* directory;

		DirectoryCloser(DIR* directory) : directory(directory) {}

		~DirectoryCloser() {
			if(directory)
				closedir(directory);
		}

	};

	void Filesystem::readdir(const string& path, Appender<string>& sink) {
		DIR* dir = opendir(path.c_str());
		if(!dir)
			bail();
		struct dirent* ent;
		DirectoryCloser closer(dir);
		while((ent = ::readdir(dir))) {
			if(strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
				sink.append(ent->d_name);
		}
		closer.directory = NULL;
		if(closedir(dir))
			bail();
		sink.doneAppending();
	}

	void Filesystem::truncate(const string& path, size_t size) {
		if(::truncate(path.c_str(), static_cast<off_t>(size)))
			bail();
	}

	void Filesystem::statfs(const string& path, FSInfo& info) {
		struct statfs native;
		if(::statfs(path.c_str(), &native))
			bail();
		switch(native.f_bfree) {
			case 0x0000ADF5l: // ADFS_SUPER_MAGIC
				info.setType(ADFS);
				break;
			case 0x0000ADFFl: // AFFS_SUPER_MAGIC
				info.setType(AFFS);
				break;
			case 0x42465331l: // BEFS_SUPER_MAGIC
				info.setType(BEFS);
				break;
			case 0x1BADFACEl: // BFS_MAGIC
				info.setType(BFS);
				break;
			case 0xFF534D42l: // CIFS_MAGIC_NUMBER
				info.setType(CIFS);
				break;
			case 0x73757245l: // CODA_SUPER_MAGIC
				info.setType(CODA);
				break;
			case 0x012FF7B7l: // COH_SUPER_MAGIC
				info.setType(COH);
				break;
			case 0x28CD3D45l: // CRAMFS_MAGIC
				info.setType(CRAMFS);
				break;
			case 0x00001373l: // DEVFS_SUPER_MAGIC
				info.setType(DEVFS);
				break;
			case 0x00414A53l: // EFS_SUPER_MAGIC
				info.setType(EFS);
				break;
			case 0x0000137Dl: // EXT_SUPER_MAGIC
				info.setType(EXT);
				break;
			case 0x0000EF51l: // EXT2_OLD_SUPER_MAGIC
			case 0x0000EF53l: // EXT2_SUPER_MAGIC
				// 0xEF53 is also EXT3_SUPER_MAGIC, so we can' really distinguish
				info.setType(EXT2);
				break;
			case 0x00004244l: // HFS_SUPER_MAGIC
				info.setType(HFS);
				break;
			case 0xF995E849l: // HPFS_SUPER_MAGIC
				info.setType(HPFS);
				break;
			case 0x958458F6l: // HUGETLBFS_MAGIC
				info.setType(HUGETLBFS);
				break;
			case 0x00009660l: // ISOFS_SUPER_MAGIC
				info.setType(ISOFS);
				break;
			case 0x000072b6l: // JFFS2_SUPER_MAGIC
				info.setType(JFFS2);
				break;
			case 0x3153464Al: // JFS_SUPER_MAGIC
				info.setType(JFS);
				break;
			case 0x0000137Fl: // MINIX_SUPER_MAGIC
			case 0x0000138Fl: // MINIX_SUPER_MAGIC2
				info.setType(MINIX);
				break;
			case 0x00002468l: // MINIX2_SUPER_MAGIC
			case 0x00002478l: // MINIX2_SUPER_MAGIC2
				info.setType(MINIX2);
				break;
			case 0x00004D44l: // MSDOS_SUPER_MAGIC
				info.setType(MSDOS);
				break;
			case 0x0000564Cl: // NCP_SUPER_MAGIC
				info.setType(NCP);
				break;
			case 0x00006969l: // NFS_SUPER_MAGIC
				info.setType(NFS);
				break;
			case 0x5346544El: // NTFS_SB_MAGIC
				info.setType(NTFS);
				break;
			case 0x00009FA1l: // OPENPROM_SUPER_MAGIC
				info.setType(OPENPROM);
				break;
			case 0x00009FA0l: // PROC_SUPER_MAGIC
				info.setType(PROC);
				break;
			case 0x0000002Fl: // QNX4_SUPER_MAGIC
				info.setType(QNX4);
				break;
			case 0x52654973l: // REISERFS_SUPER_MAGIC
				info.setType(REISERFS);
				break;
			case 0x00007275l: // ROMFS_MAGIC
				info.setType(ROMFS);
				break;
			case 0x0000517Bl: // SMB_SUPER_MAGIC
				info.setType(SMB);
				break;
			case 0x012FF7B6l: // SYSV2_SUPER_MAGIC
				info.setType(SYSV2);
				break;
			case 0x012FF7B5l: // SYSV4_SUPER_MAGIC
				info.setType(SYSV4);
				break;
			case 0x01021994l: // TMPFS_MAGIC
				info.setType(TMPFS);
				break;
			case 0x15013346l: // UDF_SUPER_MAGIC
				info.setType(UDF);
				break;
			case 0x00011954l: // UFS_MAGIC
				info.setType(UFS);
				break;
			case 0x00009FA2l: // USBDEVICE_SUPER_MAGIC
				info.setType(USBDEV);
				break;
			case 0xA501FCF5l: // VXFS_SUPER_MAGIC
				info.setType(VXFS);
				break;
			case 0x012FF7B4l: // XENIX_SUPER_MAGIC
				info.setType(XENIX);
				break;
			case 0x58465342l: // XFS_SUPER_MAGIC
				info.setType(XFS);
				break;
			case 0x012FD16Dl: // _XIAFS_SUPER_MAGIC
				info.setType(XIAFS);
				break;
			default:
				info.setType(UNKNOWNFS);
				break;
		}
		info.setTotalBlockCount(static_cast<size_t>(native.f_blocks));
		info.setFreeBlockCount(static_cast<size_t>(native.f_bfree));
		info.setTotalINodeCount(static_cast<size_t>(native.f_files));
		info.setFreeINodeCount(static_cast<size_t>(native.f_ffree));
		info.setMaximumFilenameLength(static_cast<size_t>(native.f_namelen));
	}

	void Filesystem::listRoots(Appender<string>& sink) {
		sink.append("");
		sink.doneAppending();
	}

	void Filesystem::mknod(const string& path, Stat::Type type, int permissions, Stat::DeviceID device) {
		mode_t mode;
		switch(type) {
			case Stat::REGULAR_FILE:
				mode = static_cast<mode_t>(S_IFREG);
				break;
			case Stat::CHARACTER_DEVICE:
				mode = static_cast<mode_t>(S_IFCHR);
				break;
			case Stat::BLOCK_DEVICE:
				mode = static_cast<mode_t>(S_IFBLK);
				break;
			case Stat::NAMED_PIPE:
				mode = static_cast<mode_t>(S_IFIFO);
				break;
			case Stat::LOCAL_SOCKET:
				mode = static_cast<mode_t>(S_IFSOCK);
				break;
			default:
				throw FilesystemError(EINVAL);
		}
		mode |= static_cast<mode_t>(permissions & Stat::PERMISSIONS_MASK);
		if(::mknod(path.c_str(), mode, device))
			bail();
	}

	string Filesystem::getErrorMessage(ErrorCode error) {
		return strerror(error);
	}

	Stat::UserID Filesystem::getCurrentUser() {
		return getuid();
	}

	Stat::GroupID Filesystem::getCurrentGroup() {
		return getgid();
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	static void bail() {
		DWORD error = GetLastError();
		switch(error) {
			//case 0x00000002: // ERROR_FILE_NOT_FOUND
			case ERROR_FILE_NOT_FOUND:
			//case 0x00000003: // ERROR_PATH_NOT_FOUND
			case ERROR_PATH_NOT_FOUND:
				throw NoSuchFileError(error);
			//case 0x00000005: // ERROR_ACCESS_DENIED
			case ERROR_ACCESS_DENIED:
				throw FileAccessDeniedError(error);
			default:
				throw FilesystemError(error);
		}
	}

	struct SIDFreer {

		PSID sid;

		SIDFreer(PSID sid) : sid(sid) {}

		~SIDFreer() {
			if(sid)
				FreeSid(sid);
		}

	};

	struct LocalFreer {

		HLOCAL object;

		LocalFreer(HLOCAL object) : object(object) {}

		~LocalFreer() {
			if(object)
				LocalFree(object);
		}

	};

	struct HandleCloser {

		HANDLE handle;

		HandleCloser(HANDLE handle) : handle(handle) {}

		~HandleCloser() {
			if(handle != INVALID_HANDLE_VALUE)
				CloseHandle(handle);
		}

	};

	static void getMountPointForFilename(const string& path, string& mountPoint) {
		DWORD size = GetFullPathName(path.c_str(), static_cast<DWORD>(0u), NULL, NULL);
		if(!size)
			bail();
		DeleteArray<char> buffer(new char[size]);
		if(!GetVolumePathName(path.c_str(), *buffer, size))
			bail();
		mountPoint.assign(*buffer);
	}

	static void getVolumeIDForMountPoint(const string& mountPoint, string& volumeID) {
		char buffer[50];
		if(!GetVolumeNameForVolumeMountPoint(mountPoint.c_str(), buffer, static_cast<DWORD>(sizeof(buffer))))
			bail();
		volumeID.assign(buffer);
	}

	static time_t filetimeToTimestamp(const FILETIME& ft) {
		ULARGE_INTEGER uli;
		uli.u.LowPart = ft.dwLowDateTime;
		uli.u.HighPart = ft.dwHighDateTime;
		uli.QuadPart -= static_cast<ULONGLONG>(116444736000000000ull);
		uli.QuadPart /= static_cast<ULONGLONG>(10000000ull);
		return static_cast<time_t>(uli.QuadPart);
	}

#define READ_PERMISSIONS (FILE_READ_DATA | FILE_READ_ATTRIBUTES)
#define WRITE_PERMISSIONS (FILE_WRITE_DATA | FILE_APPEND_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA)
#define EXECUTE_PERMISSIONS (FILE_READ_DATA | FILE_EXECUTE)

	void Filesystem::stat(const string& path, Stat& info, bool ofLink) {
		/* Oh sweet jumping Haruhi! This is just insanely
		 * complicated on Windows. I sure hope this stuff
		 * actually works so I'll never have to touch this
		 * abomination ever again...
		 */
		// get type
		DWORD attrs = GetFileAttributes(path.c_str());
		if(attrs == INVALID_FILE_ATTRIBUTES)
			bail();
		if(attrs & FILE_ATTRIBUTE_REPARSE_POINT)
			info.setType(Stat::SYMBOLIC_LINK);
		else if(attrs & FILE_ATTRIBUTE_DIRECTORY)
			info.setType(Stat::DIRECTORY);
		else
			info.setType(Stat::REGULAR_FILE);
		HANDLE file = CreateFile(path.c_str(), /*GENERIC_READ |*/ READ_CONTROL, FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(file == INVALID_HANDLE_VALUE)
			bail();
		HandleCloser closer(file);
		PSID nativeOwner, nativeGroup;
		PACL acl;
		PSECURITY_DESCRIPTOR security;
		DWORD error = GetSecurityInfo(file, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION
				| GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, &nativeOwner, &nativeGroup, &acl,
				NULL, &security);
		if(error != ERROR_SUCCESS)
			bail();
		LocalFreer freeSecurity(security);
		// need not use SIDFreer on nativeOwner nor nativeGroup,
		// as the SID objects are part of the 'security' object
		// and will be freed by freeSecurity
		BY_HANDLE_FILE_INFORMATION fileInfo;
		if(!GetFileInformationByHandle(file, &fileInfo))
			bail();
		closer.handle = INVALID_HANDLE_VALUE;
		if(!CloseHandle(file))
			bail();
		LPTSTR sidString;
		// get owner
		if(!ConvertSidToStringSid(nativeOwner, &sidString))
			bail();
		LocalFreer freeSidString(sidString);
		info.setOwner(sidString);
		LocalFree(sidString);
		freeSidString.object = NULL;
		// get group
		if(!ConvertSidToStringSid(nativeGroup, &sidString))
			bail();
		freeSidString.object = sidString;
		info.setGroup(sidString);
		// get device
		string mountPoint, volumeID;
		getMountPointForFilename(path, mountPoint);
		getVolumeIDForMountPoint(mountPoint, volumeID);
		info.setDevice(volumeID);
		info.setSpecialSpecifier(Stat::NO_DEVICE);
		// get permissions
		WORD w;
		int seenAllowed = 0, seenDenied;
		for(w = static_cast<WORD>(0); w < acl->AceCount; ++w) {
			LPVOID aceAddr;
			if(!GetAce(acl, static_cast<DWORD>(w), &aceAddr))
				bail();
			union {
				ACE_HEADER* header;
				ACCESS_ALLOWED_ACE* allowed;
				ACCESS_ALLOWED_CALLBACK_ACE* allowedCallback;
				ACCESS_ALLOWED_CALLBACK_OBJECT_ACE* allowedCallbackObject;
				ACCESS_ALLOWED_OBJECT_ACE* allowedObject;
				ACCESS_DENIED_ACE* denied;
				ACCESS_DENIED_CALLBACK_ACE* deniedCallback;
				ACCESS_DENIED_CALLBACK_OBJECT_ACE* deniedCallbackObject;
				ACCESS_DENIED_OBJECT_ACE* deniedObject;
			} ace;
			ace.header = static_cast<ACE_HEADER*>(aceAddr);
			ACCESS_MASK mask;
			SID* sidStart;
			bool denies, applies;
			switch(ace.header->AceType) {
				#define clamp(type, component, mode) \
					case type: \
						mask = ace.component->Mask; \
						sidStart = reinterpret_cast<SID*>(&ace.component->SidStart); \
						denies = mode; \
						break;
				clamp(ACCESS_ALLOWED_ACE_TYPE, allowed, false)
				clamp(ACCESS_ALLOWED_CALLBACK_ACE_TYPE, allowedCallback, false)
				clamp(ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE, allowedCallbackObject, false)
				clamp(ACCESS_ALLOWED_OBJECT_ACE_TYPE, allowedObject, false)
				clamp(ACCESS_DENIED_ACE_TYPE, denied, true)
				clamp(ACCESS_DENIED_CALLBACK_ACE_TYPE, deniedCallback, true)
				clamp(ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE, deniedCallbackObject, true)
				clamp(ACCESS_DENIED_OBJECT_ACE_TYPE, deniedObject, true)
				#undef clamp
				default:
					continue;
			}
			unsigned groupShift;
			if(EqualSid(nativeOwner, sidStart))
				groupShift = 6u;
			else if(EqualSid(nativeGroup, sidStart))
				groupShift = 3u;
			else
				continue;
			#define clamp(permMask, bit) \
				if(denies) \
					applies = !!(mask & permMask); \
				else \
					applies = (mask & permMask) == permMask; \
				if(!applies) \
					continue; \
				if(denies) \
					seenDenied |= bit << groupShift; \
				else \
					seenAllowed |= bit << groupShift;
			clamp(READ_PERMISSIONS, 04)
			clamp(WRITE_PERMISSIONS, 02)
			clamp(EXECUTE_PERMISSIONS, 01)
			#undef clamp
		}
		info.setPermissions(seenAllowed & ~seenDenied);
		info.setSize(
			IntegerBits<size_t>::shiftLeft(
				static_cast<size_t>(fileInfo.nFileSizeHigh),
				static_cast<unsigned>(sizeof(DWORD)) * 8u
			) | static_cast<size_t>(fileInfo.nFileSizeLow)
		);
		info.setAccessTimestamp(filetimeToTimestamp(fileInfo.ftLastAccessTime));
		info.setModificationTimestamp(filetimeToTimestamp(fileInfo.ftLastWriteTime));
		// we'll just have to postulate that Windows doesn't have status changes,
		// and therefore ctime coincides with creation time
		info.setStatusChangeTimestamp(filetimeToTimestamp(fileInfo.ftCreationTime));
	}

	void Filesystem::chmod(const string& path, int permissions) {
		//TODO
	}

	void Filesystem::chown(const string& path, Stat::UserID owner, bool ofLink) {
		//TODO
	}

	void Filesystem::chgrp(const string& path, Stat::GroupID group, bool ofLink) {
		//TODO
	}

	void Filesystem::link(const string& oldPath, const string& newPath) {
		//TODO
	}

	void Filesystem::unlink(const string& path) {
		//TODO
	}

	void Filesystem::utime(const string& path) {
		//TODO
	}

	void Filesystem::utime(const string& path, time_t accessTimestamp, time_t modificationTimestamp) {
		//TODO
	}

	bool Filesystem::access(const string& path, int permissions) {
		//TODO
		/*
		int mode = 0;
		if(permissions == FILE_EXISTS)
			mode = F_OK;
		else {
			if(permissions & CAN_READ)
				mode |= R_OK;
			if(permissions & CAN_WRITE)
				mode |= W_OK;
			if(permissions & CAN_EXECUTE)
				mode |= X_OK;
		}
		if(!::access(path.c_str(), mode))
			return true;
		if(errno != EACCES)
			bail();
		return false;
		*/
		return false;
	}

	void Filesystem::sync() {
		//TODO
	}

	void Filesystem::rename(const string& oldPath, const string& newPath) {
		//TODO
		/*
		if(!::rename(oldPath.c_str(), newPath.c_str()))
			return;
		if(errno != EXDEV)
			bail();
		// Now we are, of course, seriously screwed.
		// We need to do the whole damn thing by hand:
		int error = movePreserving(oldPath, newPath);
		if(error) {
			errno = error;
			bail();
		}
		*/
	}

	void Filesystem::mkdir(const string& path, int permissions) {
		//TODO
	}

	void Filesystem::rmdir(const string& path) {
		//TODO
	}

	void Filesystem::symlink(const string& oldPath, const string& newPath) {
		//TODO
	}

	void Filesystem::readlink(const string& path, string& result) {
		//TODO
	}

	void Filesystem::readdir(const string& path, Appender<string>& sink) {
		//TODO
		/*
		DIR* dir = opendir(path.c_str());
		if(!dir)
			bail();
		struct dirent* ent;
		DirectoryCloser closer(dir);
		while((ent = ::readdir(dir))) {
			if(strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
				sink.append(ent->d_name);
		}
		closer.directory = NULL;
		if(closedir(dir))
			bail();
		sink.doneAppending();
		*/
	}

	void Filesystem::truncate(const string& path, size_t size) {
		//TODO
	}

	void Filesystem::statfs(const string& path, FSInfo& info) {
		//TODO
		/*
		struct statfs native;
		if(::statfs(path.c_str(), &native))
			bail();
		switch(native.f_bfree) {
			case 0x0000ADF5l: // ADFS_SUPER_MAGIC
				info.setType(ADFS);
				break;
			case 0x0000ADFFl: // AFFS_SUPER_MAGIC
				info.setType(AFFS);
				break;
			case 0x42465331l: // BEFS_SUPER_MAGIC
				info.setType(BEFS);
				break;
			case 0x1BADFACEl: // BFS_MAGIC
				info.setType(BFS);
				break;
			case 0xFF534D42l: // CIFS_MAGIC_NUMBER
				info.setType(CIFS);
				break;
			case 0x73757245l: // CODA_SUPER_MAGIC
				info.setType(CODA);
				break;
			case 0x012FF7B7l: // COH_SUPER_MAGIC
				info.setType(COH);
				break;
			case 0x28CD3D45l: // CRAMFS_MAGIC
				info.setType(CRAMFS);
				break;
			case 0x00001373l: // DEVFS_SUPER_MAGIC
				info.setType(DEVFS);
				break;
			case 0x00414A53l: // EFS_SUPER_MAGIC
				info.setType(EFS);
				break;
			case 0x0000137Dl: // EXT_SUPER_MAGIC
				info.setType(EXT);
				break;
			case 0x0000EF51l: // EXT2_OLD_SUPER_MAGIC
			case 0x0000EF53l: // EXT2_SUPER_MAGIC
				// 0xEF53 is also EXT3_SUPER_MAGIC, so we can' really distinguish
				info.setType(EXT2);
				break;
			case 0x00004244l: // HFS_SUPER_MAGIC
				info.setType(HFS);
				break;
			case 0xF995E849l: // HPFS_SUPER_MAGIC
				info.setType(HPFS);
				break;
			case 0x958458F6l: // HUGETLBFS_MAGIC
				info.setType(HUGETLBFS);
				break;
			case 0x00009660l: // ISOFS_SUPER_MAGIC
				info.setType(ISOFS);
				break;
			case 0x000072b6l: // JFFS2_SUPER_MAGIC
				info.setType(JFFS2);
				break;
			case 0x3153464Al: // JFS_SUPER_MAGIC
				info.setType(JFS);
				break;
			case 0x0000137Fl: // MINIX_SUPER_MAGIC
			case 0x0000138Fl: // MINIX_SUPER_MAGIC2
				info.setType(MINIX);
				break;
			case 0x00002468l: // MINIX2_SUPER_MAGIC
			case 0x00002478l: // MINIX2_SUPER_MAGIC2
				info.setType(MINIX2);
				break;
			case 0x00004D44l: // MSDOS_SUPER_MAGIC
				info.setType(MSDOS);
				break;
			case 0x0000564Cl: // NCP_SUPER_MAGIC
				info.setType(NCP);
				break;
			case 0x00006969l: // NFS_SUPER_MAGIC
				info.setType(NFS);
				break;
			case 0x5346544El: // NTFS_SB_MAGIC
				info.setType(NTFS);
				break;
			case 0x00009FA1l: // OPENPROM_SUPER_MAGIC
				info.setType(OPENPROM);
				break;
			case 0x00009FA0l: // PROC_SUPER_MAGIC
				info.setType(PROC);
				break;
			case 0x0000002Fl: // QNX4_SUPER_MAGIC
				info.setType(QNX4);
				break;
			case 0x52654973l: // REISERFS_SUPER_MAGIC
				info.setType(REISERFS);
				break;
			case 0x00007275l: // ROMFS_MAGIC
				info.setType(ROMFS);
				break;
			case 0x0000517Bl: // SMB_SUPER_MAGIC
				info.setType(SMB);
				break;
			case 0x012FF7B6l: // SYSV2_SUPER_MAGIC
				info.setType(SYSV2);
				break;
			case 0x012FF7B5l: // SYSV4_SUPER_MAGIC
				info.setType(SYSV4);
				break;
			case 0x01021994l: // TMPFS_MAGIC
				info.setType(TMPFS);
				break;
			case 0x15013346l: // UDF_SUPER_MAGIC
				info.setType(UDF);
				break;
			case 0x00011954l: // UFS_MAGIC
				info.setType(UFS);
				break;
			case 0x00009FA2l: // USBDEVICE_SUPER_MAGIC
				info.setType(USBDEV);
				break;
			case 0xA501FCF5l: // VXFS_SUPER_MAGIC
				info.setType(VXFS);
				break;
			case 0x012FF7B4l: // XENIX_SUPER_MAGIC
				info.setType(XENIX);
				break;
			case 0x58465342l: // XFS_SUPER_MAGIC
				info.setType(XFS);
				break;
			case 0x012FD16Dl: // _XIAFS_SUPER_MAGIC
				info.setType(XIAFS);
				break;
			default:
				info.setType(UNKNOWNFS);
				break;
		}
		info.setTotalBlockCount(static_cast<size_t>(native.f_blocks));
		info.setFreeBlockCount(static_cast<size_t>(native.f_bfree));
		info.setTotalINodeCount(static_cast<size_t>(native.f_files));
		info.setFreeINodeCount(static_cast<size_t>(native.f_ffree));
		info.setMaximumFilenameLength(static_cast<size_t>(native.f_namelen));
		*/
	}

	void Filesystem::listRoots(Appender<string>& sink) {
		//TODO
	}

	void Filesystem::mknod(const string& path, Stat::Type type, int permissions, Stat::DeviceID device) {
		//TODO
	}

	string Filesystem::getErrorMessage(ErrorCode error) {
		//TODO
		return Thread::getErrorMessage(error);
	}

	Stat::UserID Filesystem::getCurrentUser() {
		//TODO
		return "";
	}

	Stat::GroupID Filesystem::getCurrentGroup() {
		//TODO
		return "";
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

	struct RecursiveRemover : public Appender<string> {

		const string& parent;

		RecursiveRemover(const string& parent) : parent(parent) {}

		virtual void append(const string& child) {
			Filesystem::removeRecursively(parent + Pathname::SEPARATOR + child);
		}

		virtual void doneAppending() {
			Filesystem::rmdir(parent);
		}

	};

	void Filesystem::removeRecursively(const string& path) {
		Stat info;
		stat(path, info);
		if(info.getType() != Stat::DIRECTORY)
			unlink(path);
		else {
			RecursiveRemover handler(path);
			readdir(path, handler);
		}
	}

	struct RecursiveTraverser : public Appender<string> {

		const string& baseDirectory;
		Filesystem::TraversalSink& sink;

		RecursiveTraverser(const string& baseDirectory, Filesystem::TraversalSink& sink)
				: baseDirectory(baseDirectory), sink(sink) {}

		virtual void append(const string& child) {
			Filesystem::traverse(baseDirectory + Pathname::SEPARATOR + child, sink);
		}

	};

	void Filesystem::traverse(const string& baseDirectory, TraversalSink& sink) {
		Stat info;
		stat(baseDirectory, info);
		if(info.getType() == Stat::DIRECTORY) {
			if(sink.enterDirectory(baseDirectory)) {
				RecursiveTraverser handler(baseDirectory, sink);
				readdir(baseDirectory, handler);
				sink.leaveDirectory(baseDirectory, true);
			}
			else
				sink.leaveDirectory(baseDirectory, false);
		}
		else
			sink.visitFile(baseDirectory);
	}

	static void mkdirrec(const string& directory) {
		if(Filesystem::access(directory, Filesystem::FILE_EXISTS))
			return;
		mkdirrec(Pathname::dirname(directory, Pathname::LOGICAL));
		Filesystem::mkdir(directory);
	}

	void Filesystem::mkdirRecursive(const string& directory) {
		mkdirrec(Pathname::tidy(Pathname::join(Pathname::getWorkingDirectory(), directory)));
	}

}}

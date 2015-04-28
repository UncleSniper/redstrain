#include <redstrain/util/DeleteArray.hpp>

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
#endif /* OS-specific includes */

using std::string;
using redengine::util::Appender;
using redengine::util::DeleteArray;

namespace redengine {
namespace platform {

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
		if(errno != EACCES)
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

	/*
	struct SIDFreer {

		PSID sid;

		SIDFreer(PSID sid) : sid(sid) {}

		~SIDFreer() {
			if(sid)
				FreeSid(sid);
		}

	};
	*/

	struct LocalFreer {

		HLOCAL object;

		LocalFreer(HLOCAL object) : object(object) {}

		~LocalFreer() {
			if(object)
				LocalFree(object);
		}

	};

	void Filesystem::stat(const string& path, Stat& info, bool ofLink) {
		DWORD attrs = GetFileAttributes(path.c_str());
		if(attrs == INVALID_FILE_ATTRIBUTES)
			bail();
		if(attrs & FILE_ATTRIBUTE_REPARSE_POINT)
			info.setType(Stat::SYMBOLIC_LINK);
		else if(attrs & FILE_ATTRIBUTE_DIRECTORY)
			info.setType(Stat::DIRECTORY);
		else
			info.setType(State::REGULAR_FILE);
		HANDLE file = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(file == INVALID_HANDLE_VALUE)
			bail();
		PSID nativeOwner, nativeGroup;
		PACL acl;
		PSECURITY_DESCRIPTOR security;
		DWORD error = GetSecurityInfo(file, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION
				| GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, &nativeOwner, &nativeGroup, &acl,
				NULL, &security);
		if(error != ERROR_SUCCESS)
			bail();
		LocalFreer freeSecurity(security);
		//SIDFreer freeOwnerSID(nativeOwner), freeGroupSID(nativeGroup);
		if(!CloseHandle(file))
			bail();
		LPTSTR sidString;
		if(!ConvertSidToStringSid(nativeOwner, &sidString))
			bail();
		LocalFreer freeSidString(sidString);
		info.setOwner(sidString);
		LocalFree(sidString);
		freeSidString.object = NULL;
		if(!ConvertSidToStringSid(nativeGroup, &sidString))
			bail();
		freeSidString.object = sidString;
		info.setGroup(sidString);
		//TODO
		//////
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
		if(errno != EACCES)
			bail();
		return false;
	}

	void Filesystem::sync() {
		::sync();
	}

	static int clearMoveDestination(const string& newPath) {
		struct stat ninfo;
		bool newExists = !::stat(newPath.c_str(), &ninfo);
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
		if(::stat(oldPath.c_str(), &info))
			bail();
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
	}

	void Filesystem::mknod(const string& path, Stat::Type type, int permissions, Stat::DeviceID device) {
		//TODO
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

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

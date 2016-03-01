#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/MutexLocker.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
// Only needed in header
#endif /* OS-specific includes */

#include "Database.hpp"
#include "DBSemantics.hpp"
#include "DatabaseIOError.hpp"
#include "ShortBlockError.hpp"

using std::string;
using redengine::util::FileSize;
using redengine::util::MemorySize;
using redengine::util::StringUtils;
using redengine::platform::MutexLocker;
using redengine::error::ProgrammingError;

namespace redengine {
namespace zweigana {
namespace core {

	// ======== Configurator ========

	Database::Configurator::Configurator() {}

	Database::Configurator::Configurator(const Configurator&) {}

	Database::Configurator::~Configurator() {}

	// ======== Database ========

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	struct FileCloser {

		int fd;

		FileCloser(int fd) : fd(fd) {}

		~FileCloser() {
			if(fd != -1)
				close(fd);
		}

	};

	Database::Database(const string& path, DBSemantics& semantics, OpenMode mode, Configurator* configurator) {
		int modeFlags;
		switch(mode) {
			case OPEN:
				modeFlags = 0;
				break;
			case CREATE:
				modeFlags = O_CREAT | O_EXCL;
				break;
			case OPEN_OR_CREATE:
				modeFlags = O_CREAT;
				break;
			default:
				throw ProgrammingError("Unrecognized open mode: " + StringUtils::toString(static_cast<int>(mode)));
		}
		fileHandle = open(path.c_str(), modeFlags | O_RDWR | O_APPEND | O_DSYNC, static_cast<mode_t>(0600));
		if(fileHandle == -1)
			throw DatabaseIOError(DatabaseIOError::OPEN, errno);
		FileCloser closer(fileHandle);
		struct stat64 info;
		if(fstat64(fileHandle, &info))
			throw DatabaseIOError(DatabaseIOError::OPEN, errno);
		currentSize = static_cast<FileSize>(info.st_size);
		if(configurator)
			configurator->configureDatabase(*this);
		if(currentSize)
			semantics.loadDatabase(*this, static_cast<FileSize>(currentSize));
		else
			semantics.initDatabase(*this);
		closer.fd = -1;
	}

	Database::Database(const Database& dbase) : fileHandle(dbase.fileHandle) {}

	Database::~Database() {
		if(fileHandle != -1)
			::close(fileHandle);
	}

	void Database::setMMapRegion(FileSize offset, FileSize size) {
		FileSize pageSize = static_cast<FileSize>(sysconf(_SC_PAGESIZE));
		offset = (offset + pageSize - static_cast<FileSize>(1u)) / pageSize * pageSize;
		size = (size + pageSize - static_cast<FileSize>(1u)) / pageSize * pageSize;
		if(offset != mmapOffset || size != mmapSize) {
			if(mmapData) {
				if(munmap(const_cast<char*>(mmapData), static_cast<size_t>(mmapSize)))
					throw DatabaseIOError(DatabaseIOError::MMAP, errno);
				mmapData = NULL;
			}
			if(size) {
				char* newArena = static_cast<char*>(mmap64(NULL, static_cast<size_t>(size), PROT_READ, MAP_SHARED,
						fileHandle, static_cast<off64_t>(offset)));
				if(newArena == MAP_FAILED) {
					mmapSize = static_cast<FileSize>(0u);
					throw DatabaseIOError(DatabaseIOError::MMAP, errno);
				}
				mmapData = newArena;
			}
			mmapOffset = offset;
			mmapSize = size;
		}
	}

	const char* Database::readBlock(char* buffer, FileSize offset, MemorySize size) const {
		if(!size)
			return NULL;
		FileSize requestedEnd = offset + static_cast<FileSize>(size);
		FileSize presentEnd = mmapOffset + mmapSize, csize = currentSize;
		if(csize < presentEnd)
			presentEnd = csize;
		if(offset >= mmapOffset && requestedEnd <= presentEnd)
			return mmapData + (offset - mmapOffset);
		char* dest = buffer;
		const FileSize requestedOffset = offset;
		const MemorySize requestedSize = size;
		do {
			ssize_t count = pread64(fileHandle, dest, static_cast<size_t>(size), static_cast<off64_t>(offset));
			if(count == static_cast<ssize_t>(-1))
				throw DatabaseIOError(DatabaseIOError::READ, errno);
			if(!count)
				throw ShortBlockError(requestedOffset, requestedSize);
			dest += count;
			offset += static_cast<FileSize>(count);
			size -= static_cast<MemorySize>(count);
		} while(size);
		return buffer;
	}

	FileSize Database::writeBlock(const char* buffer, MemorySize size) {
		MutexLocker lock(writeLock);
		FileSize offset = currentSize;
		while(size) {
			ssize_t count = write(fileHandle, buffer, static_cast<size_t>(size));
			if(count == static_cast<ssize_t>(-1))
				throw DatabaseIOError(DatabaseIOError::WRITE, errno);
			buffer += count;
			size -= static_cast<MemorySize>(count);
		}
		lock.release();
		return offset;
	}

	void Database::sync() {
		if(fileHandle != -1 && fdatasync(fileHandle))
			throw DatabaseIOError(DatabaseIOError::SYNC, errno);
	}

	void Database::close() {
		if(fileHandle == -1)
			return;
		if(mmapData) {
			if(munmap(const_cast<char*>(mmapData), static_cast<size_t>(mmapSize)))
				throw DatabaseIOError(DatabaseIOError::MMAP, errno);
			mmapData = NULL;
		}
		if(::close(fileHandle))
			throw DatabaseIOError(DatabaseIOError::CLOSE, errno);
		fileHandle = -1;
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

//TODO
#error Not implemented yet

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

}}}

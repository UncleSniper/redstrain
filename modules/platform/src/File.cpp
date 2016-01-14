#define _LARGEFILE64_SOURCE
#include <redstrain/util/IntegerBits.hpp>

#include "FileIOError.hpp"
#include "FileOpenError.hpp"
#include "FileSeekError.hpp"
#include "FileTellError.hpp"
#include "FileCloseError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <string.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <stdint.h>
#include "Thread.hpp"
#endif /* OS-specific includes */

using std::string;
using redengine::util::FileSize;
using redengine::util::FileOffset;
using redengine::util::MemorySize;
using redengine::util::IntegerBits;

namespace redengine {
namespace platform {

	File::File(Handle handle, Direction dir) : handle(handle), dir(dir) {}

	File::File(const File& file) : handle(file.handle), dir(file.dir) {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	File::File(const string& path, Direction dir, bool create, TruncateMode truncate) : handle(INVALID_HANDLE) {
		open(path, dir, create, truncate);
	}

	void File::open(const string& path, Direction dir, bool create, TruncateMode truncate) {
		if(handle != INVALID_HANDLE)
			close();
		this->dir = dir;
		int flags;
		switch(dir) {
			case INPUT:
				flags = O_RDONLY;
				if(truncate & TRUNCATE_IF_RDONLY)
					flags |= O_TRUNC;
				break;
			case OUTPUT:
				flags = O_WRONLY;
				if(truncate & TRUNCATE_IF_WRONLY)
					flags |= O_TRUNC;
				break;
			default:
				flags = O_RDWR;
				if(truncate & TRUNCATE_IF_RDWR)
					flags |= O_TRUNC;
				break;
		}
		if(create)
			flags |= O_CREAT;
		handle = ::open(path.c_str(), flags, static_cast<mode_t>(0644));
		if(handle == INVALID_HANDLE)
			throw FileOpenError(path, dir, errno);
	}

	void File::close() {
		if(::close(handle))
			throw FileCloseError(dir, errno);
		handle = INVALID_HANDLE;
	}

	void File::_uncheckedClose() {
		if(handle != INVALID_HANDLE) {
			::close(handle);
			handle = INVALID_HANDLE;
		}
	}

	MemorySize File::read(char* buffer, MemorySize size) {
		ssize_t count = ::read(handle, buffer, static_cast<size_t>(size));
		if(count == static_cast<ssize_t>(-1))
			throw FileIOError(INPUT, errno);
		return static_cast<MemorySize>(count);
	}

	void File::write(const char* buffer, MemorySize size) {
		while(size) {
			ssize_t count = ::write(handle, buffer, static_cast<size_t>(size));
			if(count == static_cast<ssize_t>(-1))
				throw FileIOError(OUTPUT, errno);
			buffer += count;
			size -= static_cast<MemorySize>(count);
		}
	}

	void File::seek(FileOffset offset, SeekWhence whence) {
		int nativeWhence;
		switch(whence) {
			case OFFSET_FROM_END:
				nativeWhence = SEEK_END;
				break;
			case OFFSET_FROM_HERE:
				nativeWhence = SEEK_CUR;
				break;
			default:
				nativeWhence = SEEK_SET;
				break;
		}
		if(lseek64(handle, static_cast<off64_t>(offset), nativeWhence) == static_cast<off64_t>(-1))
			throw FileSeekError(dir, errno);
	}

	FileSize File::tell() const {
		off64_t offset = lseek64(handle, static_cast<off64_t>(0), SEEK_CUR);
		if(offset == static_cast<off64_t>(-1))
			throw FileTellError(dir, errno);
		return static_cast<FileSize>(offset);
	}

	string File::getErrorMessage(ErrorCode error) {
		return strerror(error);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	const File::Handle File::INVALID_HANDLE = INVALID_HANDLE_VALUE;

	File::File(const string& path, Direction dir, bool create, TruncateMode truncate) : handle(INVALID_HANDLE) {
		open(path, dir, create, truncate);
	}

	static DWORD getOpenDisposition(bool create, TruncateMode truncate, TruncateMode tmask) {
		if(create)
			return truncate & tmask ? CREATE_ALWAYS : OPEN_ALWAYS;
		else
			return truncate & tmask ? TRUNCATE_EXISTING : OPEN_EXISTING;
	}

	void File::open(const string& path, Direction dir, bool create, TruncateMode truncate) {
		if(handle != INVALID_HANDLE)
			close();
		this->dir = dir;
		DWORD access, disposition;
		switch(dir) {
			case INPUT:
				access = GENERIC_READ;
				disposition = getOpenDisposition(create, truncate, TRUNCATE_IF_RDONLY);
				break;
			case OUTPUT:
				access = GENERIC_WRITE;
				disposition = getOpenDisposition(create, truncate, TRUNCATE_IF_WRONLY);
				break;
			default:
				access = GENERIC_READ | GENERIC_WRITE;
				disposition = getOpenDisposition(create, truncate, TRUNCATE_IF_RDWR);
				break;
		}
		handle = CreateFile(path.c_str(), access, static_cast<DWORD>(0u), NULL, disposition,
				FILE_ATTRIBUTE_NORMAL, static_cast<DWORD>(0u));
		if(handle == INVALID_HANDLE)
			throw FileOpenError(path, dir, GetLastError());
	}

	void File::close() {
		if(!CloseHandle(handle))
			throw FileCloseError(dir, GetLastError());
		handle = INVALID_HANDLE;
	}

	void File::_uncheckedClose() {
		if(handle != INVALID_HANDLE) {
			CloseHandle(handle);
			handle = INVALID_HANDLE;
		}
	}

	MemorySize File::read(char* buffer, MemorySize size) {
		DWORD count;
		if(!ReadFile(handle, buffer, static_cast<DWORD>(size), &count, NULL))
			throw FileIOError(File::INPUT, GetLastError());
		return static_cast<MemorySize>(count);
	}

	void File::write(const char* buffer, MemorySize size) {
		DWORD count;
		while(size) {
			if(!WriteFile(handle, buffer, static_cast<DWORD>(size), &count, NULL))
				throw FileIOError(File::OUTPUT, GetLastError());
			buffer += count;
			size -= static_cast<MemorySize>(count);
		}
	}

	void File::seek(FileOffset offset, SeekWhence whence) {
		DWORD nativeWhence;
		switch(whence) {
			case OFFSET_FROM_END:
				nativeWhence = FILE_END;
				break;
			case OFFSET_FROM_HERE:
				nativeWhence = FILE_CURRENT;
				break;
			default:
				nativeWhence = FILE_BEGIN;
				break;
		}
		LONG loOffset = IntegerBits<FileOffset>::getBitsAs<LONG>(offset, 32u, 0u);
		LONG hiOffset = IntegerBits<FileOffset>::getBitsAs<LONG>(offset, 32u, 1u);
		SetLastError(static_cast<DWORD>(0u));
		DWORD newLo = SetFilePointer(handle, loOffset, &hiOffset, nativeWhence);
		DWORD error = GetLastError();
		if(newLo == INVALID_SET_FILE_POINTER && error)
			throw FileSeekError(dir, error);
	}

	FileSize File::tell() const {
		LONG hiOffset = static_cast<LONG>(0);
		SetLastError(static_cast<DWORD>(0u));
		DWORD newLo = SetFilePointer(handle, static_cast<LONG>(0u), &hiOffset, FILE_CURRENT);
		DWORD error = GetLastError();
		if(newLo == INVALID_SET_FILE_POINTER && error)
			throw FileTellError(dir, error);
		return static_cast<FileSize>(IntegerBits<FileSize>::shiftLeft(
			static_cast<FileSize>(static_cast<IntegerBits<LONG>::AsUnsigned>(hiOffset)),
			32u
		) | static_cast<FileSize>(newLo));
	}

	string File::getErrorMessage(ErrorCode error) {
		return Thread::getErrorMessage(error);
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

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
using redengine::util::IntegerBits;

namespace redengine {
namespace platform {

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	File::File(const string& path, Direction dir, bool create) : handle(INVALID_HANDLE) {
		open(path, dir, create);
	}

	void File::open(const string& path, Direction dir, bool create) {
		if(handle != INVALID_HANDLE)
			close();
		this->dir = dir;
		int flags;
		switch(dir) {
			case INPUT:
				flags = O_RDONLY;
				break;
			case OUTPUT:
				flags = O_WRONLY;
				if(create)
					flags |= O_TRUNC;
				break;
			default:
				flags = O_RDWR;
				break;
		}
		if(create)
			flags |= O_CREAT;
		handle = ::open(path.c_str(), flags);
		if(handle == INVALID_HANDLE)
			throw FileOpenError(dir, errno);
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

	size_t File::read(char* buffer, size_t size) {
		ssize_t count = ::read(handle, buffer, size);
		if(count == static_cast<ssize_t>(-1))
			throw FileIOError(INPUT, errno);
		return static_cast<size_t>(count);
	}

	void File::write(const char* buffer, size_t size) {
		while(size) {
			ssize_t count = ::write(handle, buffer, size);
			if(count == static_cast<ssize_t>(-1))
				throw FileIOError(OUTPUT, errno);
			buffer += count;
			size -= static_cast<size_t>(count);
		}
	}

	void File::seek(off_t offset, SeekWhence whence) {
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
		if(lseek(handle, offset, nativeWhence) == static_cast<off_t>(-1))
			throw FileSeekError(dir, errno);
	}

	size_t File::tell() const {
		off_t offset = lseek(handle, static_cast<off_t>(0), SEEK_CUR);
		if(offset == static_cast<off_t>(-1))
			throw FileTellError(dir, errno);
		return static_cast<size_t>(offset);
	}

	string File::getErrorMessage(ErrorCode error) {
		return strerror(error);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	const File::Handle File::INVALID_HANDLE = INVALID_HANDLE_VALUE;

	File::File(const string& path, Direction dir, bool create) : handle(INVALID_HANDLE) {
		open(path, dir, create);
	}

	void File::open(const string& path, Direction dir, bool create) {
		if(handle != INVALID_HANDLE)
			close();
		this->dir = dir;
		DWORD access, disposition;
		switch(dir) {
			case INPUT:
				access = GENERIC_READ;
				disposition = create ? OPEN_ALWAYS : OPEN_EXISTING;
				break;
			case OUTPUT:
				access = GENERIC_WRITE;
				disposition = create ? CREATE_ALWAYS : OPEN_EXISTING;
				break;
			default:
				access = GENERIC_READ | GENERIC_WRITE;
				disposition = create ? OPEN_ALWAYS : OPEN_EXISTING;
				break;
		}
		handle = CreateFile(path.c_str(), access, static_cast<DWORD>(0u), NULL, disposition,
				FILE_ATTRIBUTE_NORMAL, static_cast<DWORD>(0u));
		if(handle == INVALID_HANDLE)
			throw FileOpenError(dir, GetLastError());
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

	size_t File::read(char* buffer, size_t size) {
		DWORD count;
		if(!ReadFile(handle, buffer, static_cast<DWORD>(size), &count, NULL))
			throw FileIOError(File::INPUT, GetLastError());
		return static_cast<size_t>(count);
	}

	void File::write(const char* buffer, size_t size) {
		DWORD count;
		while(size) {
			if(!WriteFile(handle, buffer, static_cast<DWORD>(size), &count, NULL))
				throw FileIOError(File::OUTPUT, GetLastError());
			buffer += count;
			size -= static_cast<size_t>(count);
		}
	}

	void File::seek(off_t offset, SeekWhence whence) {
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
		LONG loOffset = IntegerBits<off_t>::getBitsAs<LONG>(offset, 32u, 0u);
		LONG hiOffset = IntegerBits<off_t>::getBitsAs<LONG>(offset, 32u, 1u);
		SetLastError(static_cast<DWORD>(0u));
		DWORD newLo = SetFilePointer(handle, loOffset, &hiOffset, nativeWhence);
		DWORD error = GetLastError();
		if(newLo == INVALID_SET_FILE_POINTER && error)
			throw FileSeekError(dir, error);
	}

	size_t File::tell() const {
		LONG hiOffset = static_cast<LONG>(0);
		SetLastError(static_cast<DWORD>(0u));
		DWORD newLo = SetFilePointer(handle, static_cast<LONG>(0u), &hiOffset, FILE_CURRENT);
		DWORD error = GetLastError();
		if(newLo == INVALID_SET_FILE_POINTER && error)
			throw FileTellError(dir, error);
		return static_cast<size_t>(IntegerBits<size_t>::shiftLeft(
			static_cast<size_t>(static_cast<IntegerBits<LONG>::AsUnsigned>(hiOffset)),
			32u
		) | static_cast<size_t>(newLo));
	}

	string File::getErrorMessage(ErrorCode error) {
		return Thread::getErrorMessage(error);
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

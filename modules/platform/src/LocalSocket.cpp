#include "LocalSocketIOError.hpp"
#include "LocalSocketBindError.hpp"
#include "LocalSocketCloseError.hpp"
#include "LocalSocketCreateError.hpp"
#include "LocalSocketListenError.hpp"
#include "LocalSocketAcceptError.hpp"
#include "LocalSocketConnectError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include "Thread.hpp"
#endif /* OS-specific includes */

using std::string;

namespace redengine {
namespace platform {

	LocalSocket::LocalSocket(Handle handle) : handle(handle) {}

	LocalSocket::LocalSocket(const LocalSocket& socket) : StreamSocket(socket), handle(socket.handle) {}

	bool LocalSocket::isOpen() const {
		return handle != INVALID_HANDLE;
	}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	LocalSocket::LocalSocket() : handle(socket(PF_LOCAL, SOCK_STREAM, 0)) {
		if(handle == INVALID_HANDLE)
			throw LocalSocketCreateError(errno);
	}

	size_t LocalSocket::read(char* buffer, size_t size) {
		ssize_t count = ::read(handle, buffer, size);
		if(count == static_cast<ssize_t>(-1))
			throw LocalSocketIOError(SocketIOError::RECEIVE, errno);
		return static_cast<size_t>(count);
	}

	void LocalSocket::write(const char* buffer, size_t size) {
		while(size) {
			ssize_t count = ::write(handle, buffer, size);
			if(count == static_cast<ssize_t>(-1))
				throw LocalSocketIOError(SocketIOError::SEND, errno);
			buffer += count;
			size -= static_cast<size_t>(count);
		}
	}

	void LocalSocket::close() {
		if(::close(handle))
			throw LocalSocketCloseError(errno);
		handle = INVALID_HANDLE;
	}

	void LocalSocket::_uncheckedClose() {
		if(handle != INVALID_HANDLE) {
			::close(handle);
			handle = INVALID_HANDLE;
		}
	}

	void LocalSocket::listen(const string& name, unsigned backlog) {
		string path(canonizalizeSocketName(name));
		struct sockaddr_un baddr;
		baddr.sun_family = AF_UNIX;
		strncpy(baddr.sun_path, path.c_str(), sizeof(baddr.sun_path));
		baddr.sun_path[sizeof(baddr.sun_path) - static_cast<size_t>(1u)] = '\0';
		if(bind(handle, reinterpret_cast<struct sockaddr*>(&baddr), static_cast<socklen_t>(sizeof(baddr))))
			throw LocalSocketBindError(errno);
		if(::listen(handle, static_cast<int>(backlog)))
			throw LocalSocketListenError(errno);
	}

	void LocalSocket::connect(const string& name) {
		string path(canonizalizeSocketName(name));
		struct sockaddr_un caddr;
		caddr.sun_family = AF_UNIX;
		strncpy(caddr.sun_path, path.c_str(), sizeof(caddr.sun_path));
		caddr.sun_path[sizeof(caddr.sun_path) - static_cast<size_t>(1u)] = '\0';
		if(::connect(handle, reinterpret_cast<struct sockaddr*>(&caddr), static_cast<socklen_t>(sizeof(caddr))))
			throw LocalSocketConnectError(errno);
	}

	string LocalSocket::canonizalizeSocketName(const string& name) {
		if(!name.empty() && name[0] != '/')
			return "/var/run/" + name;
		return name;
	}

	LocalSocket::Handle LocalSocket::acceptHandle() {
		int fd = ::accept(handle, NULL, NULL);
		if(fd == -1)
			throw LocalSocketAcceptError(errno);
		return fd;
	}

	string LocalSocket::getErrorMessage(ErrorCode error) {
		return strerror(error);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	const LocalSocket::Handle LocalSocket::INVALID_HANDLE = INVALID_HANDLE_VALUE;

	LocalSocket::LocalSocket() : handle(INVALID_HANDLE) {}

	size_t LocalSocket::read(char* buffer, size_t size) {
		DWORD count;
		if(!ReadFile(handle, buffer, static_cast<DWORD>(size), &count, NULL))
			throw LocalSocketIOError(SocketIOError::RECEIVE, GetLastError());
		return static_cast<size_t>(count);
	}

	void LocalSocket::write(const char* buffer, size_t size) {
		DWORD count;
		while(size) {
			if(WriteFile(handle, buffer, static_cast<DWORD>(size), &count, NULL))
				throw LocalSocketIOError(SocketIOError::SEND, GetLastError());
			buffer += count;
			size -= static_cast<size_t>(count);
		}
	}

	void LocalSocket::close() {
		if(handle != INVALID_HANDLE) {
			if(!CloseHandle(handle))
				throw LocalSocketCloseError(GetLastError());
			handle = INVALID_HANDLE;
			pipeName.clear();
		}
	}

	void LocalSocket::_uncheckedClose() {
		if(handle != INVALID_HANDLE) {
			CloseHandle(handle);
			handle = INVALID_HANDLE;
			pipeName.clear();
		}
	}

	void LocalSocket::listen(const string& name, unsigned) {
		pipeName = canonizalizeSocketName(name);
		if(pipeName.length() > static_cast<string::size_type>(256u))
			pipeName = pipeName.substr(static_cast<string::size_type>(0u), static_cast<string::size_type>(256u));
		if(handle != INVALID_HANDLE) {
			CloseHandle(handle);
			handle = INVALID_HANDLE;
		}
		handle = CreateNamedPipe(pipeName.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE
				| PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, static_cast<DWORD>(512u), static_cast<DWORD>(512u),
				static_cast<DWORD>(0u), NULL);
		if(handle == INVALID_HANDLE)
			throw LocalSocketBindError(GetLastError());
	}

	void LocalSocket::connect(const string& name) {
		string path(canonizalizeSocketName(name));
		if(path.length() > static_cast<string::size_type>(256u))
			path = path.substr(static_cast<string::size_type>(0u), static_cast<string::size_type>(256u));
		if(handle != INVALID_HANDLE) {
			CloseHandle(handle);
			handle = INVALID_HANDLE;
		}
		handle = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, static_cast<HANDLE>(0u));
		if(handle == INVALID_HANDLE)
			throw LocalSocketConnectError(GetLastError());
	}

	string LocalSocket::canonizalizeSocketName(const string& name) {
		return "\\\\.\\pipe\\" + name;
	}

	LocalSocket::Handle LocalSocket::acceptHandle() {
		if(!ConnectNamedPipe(handle, NULL)) {
			if(GetLastError() != ERROR_PIPE_CONNECTED)
				throw LocalSocketAcceptError(GetLastError());
		}
		HANDLE h = handle;
		handle = CreateNamedPipe(pipeName.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE
				| PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, static_cast<DWORD>(512u), static_cast<DWORD>(512u),
				static_cast<DWORD>(0u), NULL);
		if(handle == INVALID_HANDLE) {
			CloseHandle(h);
			throw LocalSocketAcceptError(GetLastError());
		}
		return h;
	}

	string LocalSocket::getErrorMessage(ErrorCode error) {
		return Thread::getErrorMessage(error);
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

	LocalSocket LocalSocket::accept() {
		return LocalSocket(acceptHandle());
	}

	StreamSocket* LocalSocket::acceptAgnostic() {
		return new LocalSocket(acceptHandle());
	}

}}

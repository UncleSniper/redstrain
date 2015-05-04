#include "TCPSocketIOError.hpp"
#include "TCPSocketCloseError.hpp"
#include "TCPSocketAcceptError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include "Thread.hpp"
#endif /* OS-specific includes */

using std::string;

namespace redengine {
namespace platform {

	TCPSocket::TCPSocket(Handle handle) : handle(handle) {}

	TCPSocket::TCPSocket(const TCPSocket& socket) : StreamSocket(socket), handle(socket.handle) {}

	bool TCPSocket::isOpen() const {
		return handle != INVALID_HANDLE;
	}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	TCPSocket::Handle TCPSocket::acceptHandle() const {
		int fd = accept(handle, NULL, NULL);
		if(fd == -1)
			throw TCPSocketAcceptError(errno);
		return fd;
	}

	size_t TCPSocket::read(char* buffer, size_t size) {
		ssize_t count = ::read(handle, buffer, size);
		if(count == static_cast<ssize_t>(-1))
			throw TCPSocketIOError(SocketIOError::RECEIVE, errno);
		return static_cast<size_t>(count);
	}

	void TCPSocket::write(const char* buffer, size_t size) {
		while(size) {
			ssize_t count = ::write(handle, buffer, size);
			if(count == static_cast<ssize_t>(-1))
				throw TCPSocketIOError(SocketIOError::SEND, errno);
			buffer += count;
			size -= static_cast<size_t>(count);
		}
	}

	void TCPSocket::_uncheckedClose() {
		if(handle != INVALID_HANDLE) {
			::close(handle);
			handle = INVALID_HANDLE;
		}
	}

	void TCPSocket::close() {
		if(::close(handle))
			throw TCPSocketCloseError(errno);
		handle = INVALID_HANDLE;
	}

	string TCPSocket::getErrorMessage(ErrorCode error) {
		return strerror(error);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	TCPSocket::Handle TCPSocket::acceptHandle() const {
		SOCKET sock = WSAAccept(handle, NULL, NULL, NULL, static_cast<DWORD_PTR>(0u));
		if(sock == INVALID_SOCKET)
			throw TCPSocketAcceptError(WSAGetLastError());
		return sock;
	}

	size_t TCPSocket::read(char* buffer, size_t size) {
		DWORD count, flags = static_cast<DWORD>(0u);
		WSABUF bufinfo;
		bufinfo.len = static_cast<u_long>(size);
		bufinfo.buf = buffer;
		if(WSARecv(handle, &bufinfo, static_cast<DWORD>(1u), &count, &flags, NULL, NULL))
			throw TCPSocketIOError(SocketIOError::RECEIVE, WSAGetLastError());
		return static_cast<size_t>(count);
	}

	void TCPSocket::write(const char* buffer, size_t size) {
		DWORD count;
		WSABUF bufinfo;
		bufinfo.len = static_cast<u_long>(size);
		bufinfo.buf = const_cast<char*>(buffer);
		while(size) {
			if(WSASend(handle, &bufinfo, static_cast<DWORD>(1u), &count, static_cast<DWORD>(0u), NULL, NULL))
				throw TCPSocketIOError(SocketIOError::SEND, WSAGetLastError());
			bufinfo.buf += count;
			size -= static_cast<size_t>(count);
		}
	}

	void TCPSocket::close() {
		if(closesocket(handle))
			throw TCPSocketCloseError(WSAGetLastError());
		handle = INVALID_HANDLE;
	}

	void TCPSocket::_uncheckedClose() {
		if(handle != INVALID_HANDLE) {
			closesocket(handle);
			handle = INVALID_HANDLE;
		}
	}

	string TCPSocket::getErrorMessage(ErrorCode error) {
		return Thread::getErrorMessage(static_cast<Thread::ErrorCode>(error));
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

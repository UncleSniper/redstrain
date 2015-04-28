#include "DatagramSocketCloseError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <string.h>
#include <unistd.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include "Thread.hpp"
#endif /* OS-specific includes */

using std::string;

namespace redengine {
namespace platform {

	DatagramSocket::DatagramSocket(Handle handle) : handle(handle) {}

	DatagramSocket::DatagramSocket(const DatagramSocket& socket) : Socket(socket), handle(socket.handle) {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	void DatagramSocket::close() {
		if(::close(handle))
			throw DatagramSocketCloseError(errno);
		handle = INVALID_HANDLE;
	}

	void DatagramSocket::_uncheckedClose() {
		if(handle != INVALID_HANDLE) {
			::close(handle);
			handle = INVALID_HANDLE;
		}
	}

	string DatagramSocket::getErrorMessage(ErrorCode error) {
		return strerror(error);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	void DatagramSocket::close() {
		if(closesocket(handle))
			throw DatagramSocketCloseError(WSAGetLastError());
		handle = INVALID_HANDLE;
	}

	void DatagramSocket::_uncheckedClose() {
		if(handle != INVALID_HANDLE) {
			closesocket(handle);
			handle = INVALID_HANDLE;
		}
	}

	string DatagramSocket::getErrorMessage(ErrorCode error) {
		return Thread::getErrorMessage(static_cast<Thread::ErrorCode>(error));
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

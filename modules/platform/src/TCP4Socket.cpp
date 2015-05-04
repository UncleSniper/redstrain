#include "TCP4Socket.hpp"
#include "TCPSocketBindError.hpp"
#include "TCPSocketCreateError.hpp"
#include "TCPSocketListenError.hpp"
#include "TCPSocketConnectError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#if defined __FreeBSD__ || defined __OpenBSD__
#include <netinet/in_systm.h>
#endif /* BSD */
#include <netinet/in.h>
#include <netinet/ip.h>
#endif /* OS-specific includes */

namespace redengine {
namespace platform {

	TCP4Socket::TCP4Socket(Handle handle) : TCPSocket(handle) {}

	TCP4Socket::TCP4Socket(const TCP4Socket& socket) : TCPSocket(socket) {}

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	TCP4Socket::TCP4Socket() : TCPSocket(socket(PF_INET, SOCK_STREAM, 0)) {
		if(handle == INVALID_HANDLE)
			throw TCPSocketCreateError(errno);
	}

	void TCP4Socket::listen(Networking::IP4Address addr, Networking::TCPPort port, unsigned backlog) {
		struct sockaddr_in baddr;
		baddr.sin_family = AF_INET;
		baddr.sin_port = htons(port);
		baddr.sin_addr.s_addr = addr;
		if(bind(handle, reinterpret_cast<struct sockaddr*>(&baddr), static_cast<socklen_t>(sizeof(baddr))))
			throw TCPSocketBindError(errno);
		if(::listen(handle, static_cast<int>(backlog)))
			throw TCPSocketListenError(errno);
	}

	void TCP4Socket::connect(Networking::IP4Address addr, Networking::TCPPort port) {
		struct sockaddr_in caddr;
		caddr.sin_family = AF_INET;
		caddr.sin_port = htons(port);
		caddr.sin_addr.s_addr = addr;
		if(::connect(handle, reinterpret_cast<struct sockaddr*>(&caddr), static_cast<socklen_t>(sizeof(caddr))))
			throw TCPSocketConnectError(errno);
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	TCP4Socket::TCP4Socket() : TCPSocket(WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP,
			NULL, static_cast<GROUP>(0), static_cast<DWORD>(0u))) {
		if(handle == INVALID_HANDLE)
			throw TCPSocketCreateError(WSAGetLastError());
	}

	void TCP4Socket::listen(Networking::IP4Address addr, Networking::TCPPort port, unsigned backlog) {
		struct sockaddr_in baddr;
		baddr.sin_family = AF_INET;
		baddr.sin_port = htons(port);
		//baddr.sin_addr.s_addr = addr;  // internet claims it's this (same as Unix)
		baddr.sin_addr.S_un.S_addr = addr;  // MSDN claims it's this
		if(::bind(handle, reinterpret_cast<struct sockaddr*>(&baddr), static_cast<int>(sizeof(baddr))))
			throw TCPSocketBindError(WSAGetLastError());
		if(::listen(handle, static_cast<int>(backlog)))
			throw TCPSocketListenError(WSAGetLastError());
	}

	void TCP4Socket::connect(Networking::IP4Address addr, Networking::TCPPort port) {
		struct sockaddr_in caddr;
		caddr.sin_family = AF_INET;
		caddr.sin_port = htons(port);
		//caddr.sin_addr.s_addr = addr;  // internet claims it's this (same as Unix)
		caddr.sin_addr.S_un.S_addr = addr;  // MSDN claims it's this
		if(WSAConnect(handle, reinterpret_cast<struct sockaddr*>(&caddr), static_cast<int>(sizeof(caddr)),
				NULL, NULL, NULL, NULL))
			throw TCPSocketConnectError(WSAGetLastError());
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

	TCP4Socket TCP4Socket::accept() {
		return TCP4Socket(acceptHandle());
	}

	StreamSocket* TCP4Socket::acceptAgnostic() {
		return new TCP4Socket(acceptHandle());
	}

}}

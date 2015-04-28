#include "Datagram4Socket.hpp"
#include "DatagramSocketIOError.hpp"
#include "DatagramSocketBindError.hpp"
#include "DatagramSocketCreateError.hpp"
#include "SocketPacketSizeExceededError.hpp"

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

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	Datagram4Socket::Datagram4Socket() : DatagramSocket(socket(PF_INET, SOCK_DGRAM, 0)) {
		if(handle == INVALID_HANDLE)
			throw DatagramSocketCreateError(errno);
	}

	void Datagram4Socket::bind(Networking::IP4Address addr, Networking::TCPPort port) {
		struct sockaddr_in baddr;
		baddr.sin_family = AF_INET;
		baddr.sin_port = htons(port);
		baddr.sin_addr.s_addr = addr;
		if(::bind(handle, reinterpret_cast<struct sockaddr*>(&baddr), static_cast<socklen_t>(sizeof(baddr))))
			throw DatagramSocketBindError(errno);
	}

	size_t Datagram4Socket::receive(char* buffer, size_t size, Networking::IP4Address* addr,
			Networking::TCPPort* port) {
		struct sockaddr_in paddr;
		socklen_t fromlen = static_cast<socklen_t>(sizeof(paddr));
		ssize_t count = recvfrom(handle, buffer, size, MSG_TRUNC,
				reinterpret_cast<struct sockaddr*>(&paddr), &fromlen);
		if(count == static_cast<ssize_t>(-1))
			throw DatagramSocketIOError(SocketIOError::RECEIVE, errno);
		if(fromlen == static_cast<socklen_t>(sizeof(paddr))) {
			if(addr)
				*addr = paddr.sin_addr.s_addr;
			if(port)
				*port = ntohs(paddr.sin_port);
		}
		else {
			if(addr)
				*addr = Networking::ANY_ADDRESS4;
			if(port)
				*port = static_cast<Networking::TCPPort>(0u);
		}
		return static_cast<size_t>(count);
	}

	void Datagram4Socket::send(const char* buffer, size_t size, Networking::IP4Address addr,
			Networking::TCPPort port) {
		struct sockaddr_in paddr;
		paddr.sin_family = AF_INET;
		paddr.sin_port = htons(port);
		paddr.sin_addr.s_addr = addr;
		ssize_t count = sendto(handle, buffer, size, 0, reinterpret_cast<struct sockaddr*>(&paddr),
				static_cast<socklen_t>(sizeof(paddr)));
		if(count == static_cast<ssize_t>(-1)) {
			if(errno == EMSGSIZE)
				throw SocketPacketSizeExceededError(EMSGSIZE);
			throw DatagramSocketIOError(SocketIOError::SEND, errno);
		}
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	Datagram4Socket::Datagram4Socket() : DatagramSocket(WSASocket(PF_INET, SOCK_DGRAM, IPPROTO_UDP,
			NULL, static_cast<GROUP>(0), static_cast<DWORD>(0u))) {
		if(handle == INVALID_HANDLE)
			throw DatagramSocketCreateError(WSAGetLastError());
	}

	void Datagram4Socket::bind(Networking::IP4Address addr, Networking::TCPPort port) {
		struct sockaddr_in baddr;
		baddr.sin_family = AF_INET;
		baddr.sin_port = htons(port);
		//baddr.sin_addr.s_addr = addr;  // internet claims it's this (same as Unix)
		baddr.sin_addr.S_un.S_addr = addr;  // MSDN claims it's this
		if(::bind(handle, reinterpret_cast<struct sockaddr*>(&baddr), static_cast<int>(sizeof(baddr))))
			throw DatagramSocketBindError(WSAGetLastError());
	}

	size_t Datagram4Socket::receive(char* buffer, size_t size, Networking::IP4Address* addr,
			Networking::TCPPort* port) {
		struct sockaddr_in paddr;
		int fromlen = static_cast<int>(sizeof(paddr));
		DWORD count, flags = static_cast<DWORD>(0u);
		WSABUF bufinfo;
		bufinfo.len = static_cast<u_long>(size);
		bufinfo.buf = buffer;
		if(WSARecvFrom(handle, &bufinfo, static_cast<DWORD>(1u), &count, &flags,
				reinterpret_cast<struct sockaddr*>(&paddr), &fromlen, NULL, NULL))
			throw DatagramSocketIOError(SocketIOError::RECEIVE, WSAGetLastError());
		if(fromlen == static_cast<int>(sizeof(paddr))) {
			if(addr)
				//*addr = paddr.sin_addr.s_addr;  // internet claims it's this (same as Unix)
				*addr = paddr.sin_addr.S_un.S_addr;  // MSDN claims it's this
			if(port)
				*port = ntohs(paddr.sin_port);
		}
		else {
			if(addr)
				*addr = Networking::ANY_ADDRESS4;
			if(port)
				*port = static_cast<Networking::TCPPort>(0u);
		}
		return static_cast<size_t>(count);
	}

	void Datagram4Socket::send(const char* buffer, size_t size, Networking::IP4Address addr,
			Networking::TCPPort port) {
		struct sockaddr_in paddr;
		paddr.sin_family = AF_INET;
		paddr.sin_port = htons(port);
		//paddr.sin_addr.s_addr = addr;  // internet claims it's this (same as Unix)
		paddr.sin_addr.S_un.S_addr = addr;  // MSDN claims it's this
		DWORD count;
		WSABUF bufinfo;
		bufinfo.len = static_cast<u_long>(size);
		bufinfo.buf = const_cast<char*>(buffer);
		if(WSASendTo(handle, &bufinfo, static_cast<DWORD>(1u), &count, static_cast<DWORD>(0u),
				reinterpret_cast<struct sockaddr*>(&paddr), static_cast<int>(sizeof(paddr)), NULL, NULL)) {
			if(WSAGetLastError() == WSAEMSGSIZE)
				throw SocketPacketSizeExceededError(WSAEMSGSIZE);
			throw DatagramSocketIOError(SocketIOError::SEND, WSAGetLastError());
		}
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

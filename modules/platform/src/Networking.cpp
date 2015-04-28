#include "Networking.hpp"
#include "InvalidNetworkAddressError.hpp"
#include "NetworkAddressResolutionError.hpp"
#include "NetworkingInitializationError.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <netdb.h>
#include <arpa/inet.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include "Thread.hpp"
#include <ws2tcpip.h>
#endif /* OS-specific includes */

using std::string;

namespace redengine {
namespace platform {

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	string Networking::formatIP4Address(IP4Address ip4addr) {
		struct in_addr addr;
		addr.s_addr = ip4addr;
		return inet_ntoa(addr);
	}

	Networking::IP4Address Networking::parseIP4Address(const string& dotted4) {
		struct in_addr addr;
		if(!inet_aton(dotted4.c_str(), &addr))
			throw InvalidNetworkAddressError(dotted4);
		return addr.s_addr;
	}

	string Networking::nameByIP4Address(IP4Address address4) {
		struct in_addr addr;
		addr.s_addr = address4;
		struct hostent* ent = gethostbyaddr(&addr, static_cast<socklen_t>(sizeof(addr)), AF_INET);
		if(!ent)
			throw NetworkAddressResolutionError(h_errno);
		if(!ent->h_name)
			throw NetworkAddressResolutionError(HOST_NOT_FOUND);
		return ent->h_name;
	}

	Networking::IP4Address Networking::ip4AddressByName(const string& hostname) {
		struct hostent* ent = gethostbyname(hostname.c_str());
		if(!ent)
			throw NetworkAddressResolutionError(h_errno);
		if(ent->h_addrtype != AF_INET || !ent->h_addr_list || !*ent->h_addr_list)
			throw NetworkAddressResolutionError(NO_ADDRESS);
		struct in_addr* addr = *reinterpret_cast<struct in_addr**>(ent->h_addr_list);
		if(!addr)
			throw NetworkAddressResolutionError(NO_ADDRESS);
		return addr->s_addr;
	}

	string Networking::getErrorMessage(ErrorCode error) {
		switch(error) {
			case HOST_NOT_FOUND:
				return "Unknown host";
			case NO_ADDRESS:
				return "No matching address";
			case NO_RECOVERY:
				return "Non-recoverable error in name resolution";
			case TRY_AGAIN:
				return "Temporary error in name resolution";
			default:
				return "Unknown error";
		}
	}

	void Networking::initializeNetworking() {}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	string Networking::formatIP4Address(IP4Address ip4addr) {
		struct in_addr addr;
		addr.S_un.S_addr = ip4addr;
		return inet_ntoa(addr);
	}

	Networking::IP4Address Networking::parseIP4Address(const string& dotted4) {
		IP4Address addr = static_cast<IP4Address>(inet_addr(dotted4.c_str()));
		if(addr == static_cast<IP4Address>(INADDR_NONE))
			throw InvalidNetworkAddressError(dotted4);
		return addr;
	}

	string Networking::nameByIP4Address(IP4Address address4) {
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(static_cast<TCPPort>(0u));
		addr.sin_addr.S_un.S_addr = address4;
		char buffer[256];
		if(getnameinfo(reinterpret_cast<struct sockaddr*>(&addr), static_cast<socklen_t>(sizeof(addr)),
				buffer, static_cast<DWORD>(sizeof(buffer)), NULL, static_cast<DWORD>(0u),
				static_cast<DWORD>(0u)))
			throw NetworkAddressResolutionError(WSAGetLastError());
		return buffer;
	}

	Networking::IP4Address Networking::ip4AddressByName(const string& hostname) {
		struct addrinfo hints, *result, *ent;
		hints.ai_flags = 0;
		hints.ai_family = AF_INET;
		hints.ai_socktype = 0;
		hints.ai_protocol = 0;
		hints.ai_addrlen = static_cast<size_t>(0u);
		hints.ai_canonname = NULL;
		hints.ai_addr = NULL;
		hints.ai_next = NULL;
		int err = getaddrinfo(hostname.c_str(), NULL, &hints, &result);
		if(err)
			throw NetworkAddressResolutionError(err);
		for(ent = result; ent; ent = ent->ai_next) {
			if(ent->ai_family == AF_INET && ent->ai_addr) {
				IP4Address addr = reinterpret_cast<struct sockaddr_in*>(ent->ai_addr)->sin_addr.S_un.S_addr;
				freeaddrinfo(result);
				return addr;
			}
		}
		freeaddrinfo(result);
		throw NetworkAddressResolutionError(WSAHOST_NOT_FOUND);
	}

	string Networking::getErrorMessage(ErrorCode error) {
		return Thread::getErrorMessage(static_cast<DWORD>(error));
	}

	static bool wsaInitialized;

	void Networking::initializeNetworking() {
		if(wsaInitialized)
			return;
		WSAData info;
		int err = WSAStartup(MAKEWORD(2, 2), &info);
		if(err)
			throw NetworkingInitializationError(err);
		wsaInitialized = true;
	}

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

}}

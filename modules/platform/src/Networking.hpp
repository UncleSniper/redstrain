#ifndef REDSTRAIN_MOD_PLATFORM_NETWORKING_HPP
#define REDSTRAIN_MOD_PLATFORM_NETWORKING_HPP

#include <string>
#include <stdint.h>

#include "api.hpp"
#include "platform.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <sys/socket.h>
#if defined __FreeBSD__ || defined __OpenBSD__
#include <netinet/in_systm.h>
#endif /* BSD */
#include <netinet/in.h>
#include <netinet/ip.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <winsock2.h>
#include <windows.h>
#endif /* OS-specific includes */

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Networking {

	  public:
		typedef uint32_t IP4Address;
		typedef uint16_t TCPPort;
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef int ErrorCode;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		typedef int ErrorCode;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		static const IP4Address ANY_ADDRESS4 = INADDR_ANY;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		static const IP4Address ANY_ADDRESS4 = static_cast<IP4Address>(INADDR_ANY);
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific constants */

	  public:
		static std::string formatIP4Address(IP4Address);
		static IP4Address parseIP4Address(const std::string&);
		static std::string nameByIP4Address(IP4Address);
		static IP4Address ip4AddressByName(const std::string&);

		static std::string getErrorMessage(ErrorCode);
		static void initializeNetworking();

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_NETWORKING_HPP */

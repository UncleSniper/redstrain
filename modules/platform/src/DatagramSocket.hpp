#ifndef REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKET_HPP
#define REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKET_HPP

#include <string>

#include "platform.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <winsock2.h>
#endif /* OS-specific includes */

#include "Socket.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API DatagramSocket : public Socket {

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef int Handle;
		typedef int ErrorCode;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		typedef SOCKET Handle;
		typedef int ErrorCode;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		static const Handle INVALID_HANDLE = -1;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		static const Handle INVALID_HANDLE = INVALID_SOCKET;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific constants */

	  protected:
		Handle handle;

	  public:
		DatagramSocket(Handle);
		DatagramSocket(const DatagramSocket&);

		inline Handle getHandle() const {
			return handle;
		}

		virtual void close();
		virtual void _uncheckedClose();

		static std::string getErrorMessage(ErrorCode);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKET_HPP */

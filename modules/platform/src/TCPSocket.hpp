#ifndef REDSTRAIN_MOD_PLATFORM_TCPSOCKET_HPP
#define REDSTRAIN_MOD_PLATFORM_TCPSOCKET_HPP

#include "platform.hpp"

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <winsock2.h>
#endif /* OS-specific includes */

#include "StreamSocket.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCPSocket : public StreamSocket {

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
		Handle acceptHandle() const;

	  protected:
		Handle handle;

	  public:
		TCPSocket(Handle);
		TCPSocket(const TCPSocket&);

		virtual bool isOpen() const;
		virtual size_t read(char*, size_t);
		virtual void write(const char*, size_t);
		virtual void close();
		virtual void _uncheckedClose();

		static std::string getErrorMessage(ErrorCode);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCPSOCKET_HPP */

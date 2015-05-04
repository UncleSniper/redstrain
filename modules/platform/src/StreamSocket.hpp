#ifndef REDSTRAIN_MOD_PLATFORM_STREAMSOCKET_HPP
#define REDSTRAIN_MOD_PLATFORM_STREAMSOCKET_HPP

#include "Socket.hpp"
#include "SocketIOError.hpp"
#include "platform.hpp"

#if REDSTRAIN_OS == REDSTRAIN_OS_UNIX
#include <unistd.h>
#endif /* OS-specific includes */

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API StreamSocket : public Socket {

	  public:
		StreamSocket();
		StreamSocket(const StreamSocket&);

		virtual bool isOpen() const = 0;
		virtual size_t read(char*, size_t) = 0;
		virtual void write(const char*, size_t) = 0;

		virtual StreamSocket* acceptAgnostic() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_STREAMSOCKET_HPP */

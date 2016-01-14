#ifndef REDSTRAIN_MOD_PLATFORM_STREAMSOCKET_HPP
#define REDSTRAIN_MOD_PLATFORM_STREAMSOCKET_HPP

#include <redstrain/util/types.hpp>

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
		virtual util::MemorySize read(char*, util::MemorySize) = 0;
		virtual void write(const char*, util::MemorySize) = 0;

		virtual StreamSocket* acceptAgnostic() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_STREAMSOCKET_HPP */

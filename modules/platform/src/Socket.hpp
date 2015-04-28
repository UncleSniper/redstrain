#ifndef REDSTRAIN_MOD_PLATFORM_SOCKET_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKET_HPP

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Socket {

	  public:
		Socket();
		Socket(const Socket&);
		virtual ~Socket();

		virtual void close() = 0;
		virtual void _uncheckedClose() = 0;

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKET_HPP */

#ifndef REDSTRAIN_MOD_PLATFORM_TCP4SOCKET_HPP
#define REDSTRAIN_MOD_PLATFORM_TCP4SOCKET_HPP

#include "TCPSocket.hpp"
#include "Networking.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCP4Socket : public TCPSocket {

	  public:
		TCP4Socket();
		TCP4Socket(Handle);
		TCP4Socket(const TCP4Socket&);

		void listen(Networking::IP4Address, Networking::TCPPort, unsigned = 16u);
		void connect(Networking::IP4Address, Networking::TCPPort);
		TCP4Socket accept();

		virtual StreamSocket* acceptAgnostic();

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCP4SOCKET_HPP */

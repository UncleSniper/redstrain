#ifndef REDSTRAIN_MOD_PLATFORM_DATAGRAM4SOCKET_HPP
#define REDSTRAIN_MOD_PLATFORM_DATAGRAM4SOCKET_HPP

#include "Networking.hpp"
#include "DatagramSocket.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Datagram4Socket : public DatagramSocket {

	  public:
		Datagram4Socket();
		Datagram4Socket(Handle);
		Datagram4Socket(const Datagram4Socket&);

		void bind(Networking::IP4Address, Networking::TCPPort);
		size_t receive(char*, size_t, Networking::IP4Address* = NULL, Networking::TCPPort* = NULL);
		void send(const char*, size_t, Networking::IP4Address, Networking::TCPPort);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_DATAGRAM4SOCKET_HPP */

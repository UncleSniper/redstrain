#ifndef REDSTRAIN_MOD_PLATFORM_DATAGRAM4SOCKET_HPP
#define REDSTRAIN_MOD_PLATFORM_DATAGRAM4SOCKET_HPP

#include <redstrain/util/types.hpp>

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
		util::MemorySize receive(char*, util::MemorySize,
				Networking::IP4Address* = NULL, Networking::TCPPort* = NULL);
		void send(const char*, util::MemorySize, Networking::IP4Address, Networking::TCPPort);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_DATAGRAM4SOCKET_HPP */

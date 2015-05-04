#include "SocketBase.hpp"

using redengine::platform::StreamSocket;

namespace redengine {
namespace io {

	SocketBase::SocketBase(const SocketBase& stream) : Stream(stream), socket(stream.socket) {}

	SocketBase::SocketBase(StreamSocket& socket) : socket(socket) {}

	void SocketBase::close() {
		if(socket.isOpen())
			socket.close();
	}

}}

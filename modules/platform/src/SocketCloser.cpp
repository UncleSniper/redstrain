#include "SocketCloser.hpp"

namespace redengine {
namespace platform {

	SocketCloser::SocketCloser(const SocketCloser& closer) : socket(closer.socket) {}

	SocketCloser::SocketCloser(Socket& socket) : socket(socket), closed(false) {}

	SocketCloser::~SocketCloser() {
		if(!closed)
			socket._uncheckedClose();
	}

	void SocketCloser::close() {
		if(!closed) {
			socket.close();
			closed = true;
		}
	}

}}

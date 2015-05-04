#include "SocketInputStream.hpp"

using redengine::platform::StreamSocket;

namespace redengine {
namespace io {

	SocketInputStream::SocketInputStream(const SocketInputStream& stream)
			: Stream(stream), SocketBase(stream), InputStream<char>(stream) {}

	SocketInputStream::SocketInputStream(StreamSocket& socket) : SocketBase(socket) {}

	size_t SocketInputStream::readBlock(char* buffer, size_t size) {
		return socket.read(buffer, size);
	}

}}

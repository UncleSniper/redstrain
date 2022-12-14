#include "SocketInputStream.hpp"

using redengine::util::MemorySize;
using redengine::platform::StreamSocket;

namespace redengine {
namespace io {

	SocketInputStream::SocketInputStream(const SocketInputStream& stream)
			: Stream(stream), SocketBase(stream), InputStream<char>(stream) {}

	SocketInputStream::SocketInputStream(StreamSocket& socket) : SocketBase(socket) {}

	MemorySize SocketInputStream::readBlock(char* buffer, MemorySize size) {
		return socket.read(buffer, size);
	}

}}

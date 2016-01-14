#include "SocketOutputStream.hpp"

using redengine::util::MemorySize;
using redengine::platform::StreamSocket;

namespace redengine {
namespace io {

	SocketOutputStream::SocketOutputStream(const SocketOutputStream& stream)
			: Stream(stream), SocketBase(stream), OutputStream<char>(stream) {}

	SocketOutputStream::SocketOutputStream(StreamSocket& socket) : SocketBase(socket) {}

	void SocketOutputStream::writeBlock(const char* buffer, MemorySize size) {
		socket.write(buffer, size);
	}

}}

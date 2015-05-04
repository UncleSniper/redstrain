#include "SocketStream.hpp"

using redengine::platform::StreamSocket;

namespace redengine {
namespace io {

	SocketStream::SocketStream(const SocketStream& stream)
			: Stream(stream), SocketBase(stream), InputStream<char>(stream), SocketInputStream(stream),
			OutputStream<char>(stream), SocketOutputStream(stream), BidirectionalStream<char>(stream) {}

	SocketStream::SocketStream(StreamSocket& socket)
			: SocketBase(socket), SocketInputStream(socket), SocketOutputStream(socket) {}

}}

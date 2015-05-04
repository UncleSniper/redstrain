#ifndef REDSTRAIN_MOD_IO_SOCKETSTREAM_HPP
#define REDSTRAIN_MOD_IO_SOCKETSTREAM_HPP

#include "SocketInputStream.hpp"
#include "SocketOutputStream.hpp"
#include "BidirectionalStream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API SocketStream : public virtual SocketInputStream, public virtual SocketOutputStream,
			public BidirectionalStream<char> {

	  protected:
		SocketStream(const SocketStream&);

	  public:
		SocketStream(platform::StreamSocket&);

	};

}}

#endif /* REDSTRAIN_MOD_IO_SOCKETSTREAM_HPP */

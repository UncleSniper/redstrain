#ifndef REDSTRAIN_MOD_IO_SOCKETOUTPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_SOCKETOUTPUTSTREAM_HPP

#include "SocketBase.hpp"
#include "OutputStream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API SocketOutputStream : public virtual SocketBase, public virtual OutputStream<char> {

	  protected:
		SocketOutputStream(const SocketOutputStream&);

		virtual void writeBlock(const char*, util::MemorySize);

	  public:
		SocketOutputStream(platform::StreamSocket&);

	};

}}

#endif /* REDSTRAIN_MOD_IO_SOCKETOUTPUTSTREAM_HPP */

#ifndef REDSTRAIN_MOD_IO_SOCKETINPUTSTREAM_HPP
#define REDSTRAIN_MOD_IO_SOCKETINPUTSTREAM_HPP

#include "SocketBase.hpp"
#include "InputStream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API SocketInputStream : public virtual SocketBase, public virtual InputStream<char> {

	  protected:
		SocketInputStream(const SocketInputStream&);

		virtual util::MemorySize readBlock(char*, util::MemorySize);

	  public:
		SocketInputStream(platform::StreamSocket&);

	};

}}

#endif /* REDSTRAIN_MOD_IO_SOCKETINPUTSTREAM_HPP */

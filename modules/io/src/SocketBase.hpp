#ifndef REDSTRAIN_MOD_IO_SOCKETBASE_HPP
#define REDSTRAIN_MOD_IO_SOCKETBASE_HPP

#include <redstrain/platform/StreamSocket.hpp>

#include "Stream.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API SocketBase : public virtual Stream {

	  protected:
		platform::StreamSocket& socket;

	  protected:
		SocketBase(const SocketBase&);

	  public:
		SocketBase(platform::StreamSocket&);

		inline const platform::StreamSocket& getSocket() const {
			return socket;
		}

		inline platform::StreamSocket& getSocket() {
			return socket;
		}

		virtual void close();

	};

}}

#endif /* REDSTRAIN_MOD_IO_SOCKETBASE_HPP */

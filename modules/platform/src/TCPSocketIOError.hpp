#ifndef REDSTRAIN_MOD_PLATFORM_TCPSOCKETIOERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_TCPSOCKETIOERROR_HPP

#include "TCPSocket.hpp"
#include "NetworkError.hpp"
#include "SocketIOError.hpp"
#include "TCPSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCPSocketIOError
			: public SocketIOError, public TCPSocketError, public NetworkError {

	  private:
		const TCPSocket::ErrorCode code;

	  public:
		TCPSocketIOError(Direction, TCPSocket::ErrorCode);
		TCPSocketIOError(const TCPSocketIOError&);

		inline TCPSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(TCPSocketIOError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCPSOCKETIOERROR_HPP */

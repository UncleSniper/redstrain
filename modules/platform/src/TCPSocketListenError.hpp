#ifndef REDSTRAIN_MOD_PLATFORM_TCPSOCKETLISTENERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_TCPSOCKETLISTENERROR_HPP

#include "TCPSocket.hpp"
#include "TCPSocketError.hpp"
#include "SocketListenError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCPSocketListenError : public SocketListenError, public TCPSocketError {

	  private:
		const TCPSocket::ErrorCode code;

	  public:
		TCPSocketListenError(TCPSocket::ErrorCode);
		TCPSocketListenError(const TCPSocketListenError&);

		inline TCPSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(TCPSocketListenError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCPSOCKETLISTENERROR_HPP */

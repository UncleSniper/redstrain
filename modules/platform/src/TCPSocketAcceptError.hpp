#ifndef REDSTRAIN_MOD_PLATFORM_TCPSOCKETACCEPTERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_TCPSOCKETACCEPTERROR_HPP

#include "TCPSocket.hpp"
#include "NetworkError.hpp"
#include "TCPSocketError.hpp"
#include "SocketAcceptError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCPSocketAcceptError
			: public SocketAcceptError, public TCPSocketError, public NetworkError {

	  private:
		const TCPSocket::ErrorCode code;

	  public:
		TCPSocketAcceptError(TCPSocket::ErrorCode);
		TCPSocketAcceptError(const TCPSocketAcceptError&);

		inline TCPSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(TCPSocketAcceptError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCPSOCKETACCEPTERROR_HPP */

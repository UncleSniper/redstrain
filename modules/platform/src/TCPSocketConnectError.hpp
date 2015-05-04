#ifndef REDSTRAIN_MOD_PLATFORM_TCPSOCKETCONNECTERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_TCPSOCKETCONNECTERROR_HPP

#include "TCPSocket.hpp"
#include "NetworkError.hpp"
#include "TCPSocketError.hpp"
#include "SocketConnectError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCPSocketConnectError
			: public SocketConnectError, public TCPSocketError, public NetworkError {

	  private:
		const TCPSocket::ErrorCode code;

	  public:
		TCPSocketConnectError(TCPSocket::ErrorCode);
		TCPSocketConnectError(const TCPSocketConnectError&);

		inline TCPSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(TCPSocketConnectError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCPSOCKETCONNECTERROR_HPP */

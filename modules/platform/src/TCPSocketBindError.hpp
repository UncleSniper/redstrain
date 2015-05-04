#ifndef REDSTRAIN_MOD_PLATFORM_TCPSOCKETBINDERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_TCPSOCKETBINDERROR_HPP

#include "TCPSocket.hpp"
#include "TCPSocketError.hpp"
#include "SocketBindError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCPSocketBindError : public SocketBindError, public TCPSocketError {

	  private:
		const TCPSocket::ErrorCode code;

	  public:
		TCPSocketBindError(TCPSocket::ErrorCode);
		TCPSocketBindError(const TCPSocketBindError&);

		inline TCPSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(TCPSocketBindError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCPSOCKETBINDERROR_HPP */

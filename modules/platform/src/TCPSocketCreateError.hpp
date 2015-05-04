#ifndef REDSTRAIN_MOD_PLATFORM_TCPSOCKETCREATEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_TCPSOCKETCREATEERROR_HPP

#include "TCPSocket.hpp"
#include "TCPSocketError.hpp"
#include "SocketCreateError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCPSocketCreateError : public SocketCreateError, public TCPSocketError {

	  private:
		const TCPSocket::ErrorCode code;

	  public:
		TCPSocketCreateError(TCPSocket::ErrorCode);
		TCPSocketCreateError(const TCPSocketCreateError&);

		inline TCPSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(TCPSocketCreateError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCPSOCKETCREATEERROR_HPP */

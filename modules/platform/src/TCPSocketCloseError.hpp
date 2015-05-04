#ifndef REDSTRAIN_MOD_PLATFORM_TCPSOCKETCLOSEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_TCPSOCKETCLOSEERROR_HPP

#include "TCPSocket.hpp"
#include "TCPSocketError.hpp"
#include "SocketCloseError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCPSocketCloseError : public SocketCloseError, public TCPSocketError {

	  private:
		const TCPSocket::ErrorCode code;

	  public:
		TCPSocketCloseError(TCPSocket::ErrorCode);
		TCPSocketCloseError(const TCPSocketCloseError&);

		inline TCPSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(TCPSocketCloseError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCPSOCKETCLOSEERROR_HPP */

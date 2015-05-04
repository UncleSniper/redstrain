#ifndef REDSTRAIN_MOD_PLATFORM_LOCALSOCKETCONNECTERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCALSOCKETCONNECTERROR_HPP

#include "LocalSocket.hpp"
#include "LocalSocketError.hpp"
#include "SocketConnectError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LocalSocketConnectError : public SocketConnectError, public LocalSocketError {

	  private:
		const LocalSocket::ErrorCode code;

	  public:
		LocalSocketConnectError(LocalSocket::ErrorCode);
		LocalSocketConnectError(const LocalSocketConnectError&);

		inline LocalSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(LocalSocketConnectError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCALSOCKETCONNECTERROR_HPP */

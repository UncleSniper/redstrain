#ifndef REDSTRAIN_MOD_PLATFORM_LOCALSOCKETACCEPTERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCALSOCKETACCEPTERROR_HPP

#include "LocalSocket.hpp"
#include "LocalSocketError.hpp"
#include "SocketAcceptError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LocalSocketAcceptError : public SocketAcceptError, public LocalSocketError {

	  private:
		const LocalSocket::ErrorCode code;

	  public:
		LocalSocketAcceptError(LocalSocket::ErrorCode);
		LocalSocketAcceptError(const LocalSocketAcceptError&);

		inline LocalSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(LocalSocketAcceptError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCALSOCKETACCEPTERROR_HPP */

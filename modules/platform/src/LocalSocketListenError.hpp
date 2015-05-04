#ifndef REDSTRAIN_MOD_PLATFORM_LOCALSOCKETLISTENERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCALSOCKETLISTENERROR_HPP

#include "LocalSocket.hpp"
#include "LocalSocketError.hpp"
#include "SocketListenError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LocalSocketListenError : public SocketListenError, public LocalSocketError {

	  private:
		const LocalSocket::ErrorCode code;

	  public:
		LocalSocketListenError(LocalSocket::ErrorCode);
		LocalSocketListenError(const LocalSocketListenError&);

		inline LocalSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(LocalSocketListenError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCALSOCKETLISTENERROR_HPP */

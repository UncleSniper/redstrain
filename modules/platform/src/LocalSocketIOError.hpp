#ifndef REDSTRAIN_MOD_PLATFORM_LOCALSOCKETIOERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCALSOCKETIOERROR_HPP

#include "LocalSocket.hpp"
#include "SocketIOError.hpp"
#include "LocalSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LocalSocketIOError : public SocketIOError, public LocalSocketError {

	  private:
		const LocalSocket::ErrorCode code;

	  public:
		LocalSocketIOError(Direction, LocalSocket::ErrorCode);
		LocalSocketIOError(const LocalSocketIOError&);

		inline LocalSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(LocalSocketIOError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCALSOCKETIOERROR_HPP */

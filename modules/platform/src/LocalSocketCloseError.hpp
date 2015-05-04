#ifndef REDSTRAIN_MOD_PLATFORM_LOCALSOCKETCLOSEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCALSOCKETCLOSEERROR_HPP

#include "LocalSocket.hpp"
#include "SocketCloseError.hpp"
#include "LocalSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LocalSocketCloseError : public SocketCloseError, public LocalSocketError {

	  private:
		const LocalSocket::ErrorCode code;

	  public:
		LocalSocketCloseError(LocalSocket::ErrorCode);
		LocalSocketCloseError(const LocalSocketCloseError&);

		inline LocalSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(LocalSocketCloseError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCALSOCKETCLOSEERROR_HPP */

#ifndef REDSTRAIN_MOD_PLATFORM_LOCALSOCKETCREATEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCALSOCKETCREATEERROR_HPP

#include "LocalSocket.hpp"
#include "LocalSocketError.hpp"
#include "SocketCreateError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LocalSocketCreateError : public SocketCreateError, public LocalSocketError {

	  private:
		const LocalSocket::ErrorCode code;

	  public:
		LocalSocketCreateError(LocalSocket::ErrorCode);
		LocalSocketCreateError(const LocalSocketCreateError&);

		inline LocalSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(LocalSocketCreateError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCALSOCKETCREATEERROR_HPP */

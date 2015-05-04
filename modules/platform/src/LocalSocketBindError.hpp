#ifndef REDSTRAIN_MOD_PLATFORM_LOCALSOCKETBINDERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCALSOCKETBINDERROR_HPP

#include "LocalSocket.hpp"
#include "SocketBindError.hpp"
#include "LocalSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LocalSocketBindError : public SocketBindError, public LocalSocketError {

	  private:
		const LocalSocket::ErrorCode code;

	  public:
		LocalSocketBindError(LocalSocket::ErrorCode);
		LocalSocketBindError(const LocalSocketBindError&);

		inline LocalSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(LocalSocketBindError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCALSOCKETBINDERROR_HPP */

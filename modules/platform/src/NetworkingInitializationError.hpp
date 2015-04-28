#ifndef REDSTRAIN_MOD_PLATFORM_NETWORKINGINITIALIZATIONERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_NETWORKINGINITIALIZATIONERROR_HPP

#include "Networking.hpp"
#include "OSBindError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API NetworkingInitializationError : public OSBindError {

	  private:
		const Networking::ErrorCode code;

	  public:
		NetworkingInitializationError(Networking::ErrorCode);
		NetworkingInitializationError(const NetworkingInitializationError&);

		inline Networking::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(NetworkingInitializationError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_NETWORKINGINITIALIZATIONERROR_HPP */

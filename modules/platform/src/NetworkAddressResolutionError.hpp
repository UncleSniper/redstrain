#ifndef REDSTRAIN_MOD_PLATFORM_NETWORKADDRESSRESOLUTIONERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_NETWORKADDRESSRESOLUTIONERROR_HPP

#include "Networking.hpp"
#include "NetworkError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API NetworkAddressResolutionError : public NetworkError {

	  private:
		const Networking::ErrorCode code;

	  public:
		NetworkAddressResolutionError(Networking::ErrorCode);
		NetworkAddressResolutionError(const NetworkAddressResolutionError&);

		inline Networking::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(NetworkAddressResolutionError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_NETWORKADDRESSRESOLUTIONERROR_HPP */

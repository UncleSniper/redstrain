#ifndef REDSTRAIN_MOD_PLATFORM_INVALIDNETWORKADDRESSERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_INVALIDNETWORKADDRESSERROR_HPP

#include "NetworkError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API InvalidNetworkAddressError : public NetworkError {

	  private:
		const std::string addr;

	  public:
		InvalidNetworkAddressError(const std::string&);
		InvalidNetworkAddressError(const InvalidNetworkAddressError&);

		inline const std::string& getAddress() const {
			return addr;
		}

		REDSTRAIN_DECLARE_ERROR(InvalidNetworkAddressError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_INVALIDNETWORKADDRESSERROR_HPP */

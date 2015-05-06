#include "InvalidNetworkAddressError.hpp"

namespace redengine {
namespace platform {

	InvalidNetworkAddressError::InvalidNetworkAddressError(const std::string& addr) : addr(addr) {}

	InvalidNetworkAddressError::InvalidNetworkAddressError(const InvalidNetworkAddressError& error)
			: Error(error), IOError(error), NetworkError(error), addr(error.addr) {}

	REDSTRAIN_DEFINE_ERROR(InvalidNetworkAddressError) {
		out << "Invalid network address: " << addr;
	}

}}

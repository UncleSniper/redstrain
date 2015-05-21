#include "NumberAddressSpaceError.hpp"

using std::string;

namespace redengine {
namespace io {

	NumberAddressSpaceError::NumberAddressSpaceError(const string& number) : IllegalArgumentError(number) {}

	NumberAddressSpaceError::NumberAddressSpaceError(const NumberAddressSpaceError& error)
			: IllegalArgumentError(error) {}

	REDSTRAIN_DEFINE_ERROR(NumberAddressSpaceError) {
		out << "Number exceeds addressable range: " << getRawMessage();
	}

}}

#include "NumberAddressSpaceError.hpp"

using std::string;

namespace redengine {
namespace io {

	NumberAddressSpaceError::NumberAddressSpaceError(const string& rendition) : rendition(rendition) {}

	NumberAddressSpaceError::NumberAddressSpaceError(const NumberAddressSpaceError& error)
			: Error(error), IllegalArgumentError(error), rendition(error.rendition) {}

	REDSTRAIN_DEFINE_ERROR(NumberAddressSpaceError) {
		out << "Number exceeds addressable range: " << rendition;
	}

}}

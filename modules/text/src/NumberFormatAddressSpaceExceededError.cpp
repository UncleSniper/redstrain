#include "NumberFormatAddressSpaceExceededError.hpp"

namespace redengine {
namespace text {

	NumberFormatAddressSpaceExceededError::NumberFormatAddressSpaceExceededError() {}

	NumberFormatAddressSpaceExceededError::NumberFormatAddressSpaceExceededError(const
			NumberFormatAddressSpaceExceededError& error) : FormattingError(error) {}

	REDSTRAIN_DEFINE_ERROR(NumberFormatAddressSpaceExceededError) {
		out << "Supplied numeric value exceeds address space of requested formatting";
	}

}}

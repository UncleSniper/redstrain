#include "AddressSpaceExceededError.hpp"

namespace redengine {
namespace protostr {

	AddressSpaceExceededError::AddressSpaceExceededError() {}

	AddressSpaceExceededError::AddressSpaceExceededError(const AddressSpaceExceededError& error)
			: Error(error), ProtocolError(error) {}

	REDSTRAIN_DEFINE_ERROR(AddressSpaceExceededError) {
		out << "Element size exceeds wire address space";
	}

}}

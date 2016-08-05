#include "NoElementInThisStateError.hpp"

namespace redengine {
namespace algorithm {

	NoElementInThisStateError::NoElementInThisStateError() {}

	NoElementInThisStateError::NoElementInThisStateError(const NoElementInThisStateError& error)
			: Error(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(NoElementInThisStateError) {
		out << "No element available in current state";
	}

}}

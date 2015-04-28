#include "InconsistentCallNestingError.hpp"

namespace redengine {
namespace error {

	InconsistentCallNestingError::InconsistentCallNestingError() {}

	InconsistentCallNestingError::InconsistentCallNestingError(const InconsistentCallNestingError& error)
			: Error(error), StateError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(InconsistentCallNestingError)

}}

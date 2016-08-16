#include "OutOfComputationalCapacityError.hpp"

namespace redengine {
namespace parallel {

	OutOfComputationalCapacityError::OutOfComputationalCapacityError() {}

	OutOfComputationalCapacityError::OutOfComputationalCapacityError(const OutOfComputationalCapacityError& error)
			: Error(error), ParallelError(error) {}

	REDSTRAIN_DEFINE_ERROR(OutOfComputationalCapacityError) {
		out << "Out of computational capacity";
	}

}}

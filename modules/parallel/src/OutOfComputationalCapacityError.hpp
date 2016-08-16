#ifndef REDSTRAIN_MOD_PARALLEL_OUTOFCOMPUTATIONALCAPACITYERROR_HPP
#define REDSTRAIN_MOD_PARALLEL_OUTOFCOMPUTATIONALCAPACITYERROR_HPP

#include "ParallelError.hpp"

namespace redengine {
namespace parallel {

	class REDSTRAIN_PARALLEL_API OutOfComputationalCapacityError : public ParallelError {

	  public:
		OutOfComputationalCapacityError();
		OutOfComputationalCapacityError(const OutOfComputationalCapacityError&);

		REDSTRAIN_DECLARE_ERROR(OutOfComputationalCapacityError)

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_OUTOFCOMPUTATIONALCAPACITYERROR_HPP */

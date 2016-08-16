#ifndef REDSTRAIN_MOD_PARALLEL_PARALLELERROR_HPP
#define REDSTRAIN_MOD_PARALLEL_PARALLELERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace parallel {

	class REDSTRAIN_PARALLEL_API ParallelError : public virtual error::Error {

	  public:
		ParallelError();
		ParallelError(const ParallelError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(ParallelError)

	};

}}

#endif /* REDSTRAIN_MOD_PARALLEL_PARALLELERROR_HPP */

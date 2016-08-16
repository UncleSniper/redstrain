#include "ParallelError.hpp"

namespace redengine {
namespace parallel {

	ParallelError::ParallelError() {}

	ParallelError::ParallelError(const ParallelError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(ParallelError)

}}

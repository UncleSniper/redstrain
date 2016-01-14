#ifndef REDSTRAIN_MOD_ALGORITHM_ALLOCATORS_HPP
#define REDSTRAIN_MOD_ALGORITHM_ALLOCATORS_HPP

#include <cstddef>
#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace algorithm {

	REDSTRAIN_ALGORITHM_API void* standardAlloc(util::MemorySize);

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_ALLOCATORS_HPP */

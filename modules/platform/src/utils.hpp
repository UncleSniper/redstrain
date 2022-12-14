#ifndef REDSTRAIN_MOD_PLATFORM_UTILS_HPP
#define REDSTRAIN_MOD_PLATFORM_UTILS_HPP

#include <stdint.h>

#include "api.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_PLATFORM_API uint64_t currentTimeMillis();
	REDSTRAIN_PLATFORM_API unsigned numberOfProcessors();

}}

#endif /* REDSTRAIN_MOD_PLATFORM_UTILS_HPP */

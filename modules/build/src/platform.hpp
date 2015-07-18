#ifndef REDSTRAIN_MOD_BUILD_PLATFORM_HPP
#define REDSTRAIN_MOD_BUILD_PLATFORM_HPP

#include <redstrain/redmond/constants.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	REDSTRAIN_BUILD_API bool requiresPositionIndependentCode(redmond::Architecture);

}}

#endif /* REDSTRAIN_MOD_BUILD_PLATFORM_HPP */

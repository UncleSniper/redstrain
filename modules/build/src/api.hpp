#ifndef REDSTRAIN_MOD_BUILD_API_HPP
#define REDSTRAIN_MOD_BUILD_API_HPP

#include <redstrain/redmond/macros.hpp>

#ifdef REDSTRAIN_NEEDS_DLLEXPORT
#ifdef BUILDING_REDSTRAIN_BUILD_API
#define REDSTRAIN_BUILD_API __declspec(dllexport)
#else /* !BUILDING_REDSTRAIN_BUILD_API */
#define REDSTRAIN_BUILD_API __declspec(dllimport)
#endif /* BUILDING_REDSTRAIN_BUILD_API */
#else /* !REDSTRAIN_NEEDS_DLLEXPORT */
#define REDSTRAIN_BUILD_API
#endif

#endif /* REDSTRAIN_MOD_BUILD_API_HPP */

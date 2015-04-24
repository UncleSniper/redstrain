#ifndef REDSTRAIN_MOD_UTIL_API_HPP
#define REDSTRAIN_MOD_UTIL_API_HPP

#include <redstrain/redmond/macros.hpp>

#ifdef REDSTRAIN_NEEDS_DLLEXPORT
#ifdef BUILDING_REDSTRAIN_UTIL_API
#define REDSTRAIN_UTIL_API __declspec(dllexport)
#else /* !BUILDING_REDSTRAIN_UTIL_API */
#define REDSTRAIN_UTIL_API __declspec(dllimport)
#endif /* BUILDING_REDSTRAIN_UTIL_API */
#else /* !REDSTRAIN_NEEDS_DLLEXPORT */
#define REDSTRAIN_UTIL_API
#endif

#endif /* REDSTRAIN_MOD_UTIL_API_HPP */

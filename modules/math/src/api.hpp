#ifndef REDSTRAIN_MOD_MATH_API_HPP
#define REDSTRAIN_MOD_MATH_API_HPP

#include <redstrain/redmond/macros.hpp>

#ifdef REDSTRAIN_NEEDS_DLLEXPORT
#ifdef BUILDING_REDSTRAIN_MATH_API
#define REDSTRAIN_MATH_API __declspec(dllexport)
#else /* !BUILDING_REDSTRAIN_MATH_API */
#define REDSTRAIN_MATH_API __declspec(dllimport)
#endif /* BUILDING_REDSTRAIN_MATH_API */
#else /* !REDSTRAIN_NEEDS_DLLEXPORT */
#define REDSTRAIN_MATH_API
#endif

#endif /* REDSTRAIN_MOD_MATH_API_HPP */

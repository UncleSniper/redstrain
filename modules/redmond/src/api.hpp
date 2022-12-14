#ifndef REDSTRAIN_MOD_REDMOND_API_HPP
#define REDSTRAIN_MOD_REDMOND_API_HPP

#include "macros.hpp"

#ifdef REDSTRAIN_NEEDS_DLLEXPORT
#ifdef BUILDING_REDSTRAIN_REDMOND_API
#define REDSTRAIN_REDMOND_API __declspec(dllexport)
#else /* !BUILDING_REDSTRAIN_REDMOND_API */
#define REDSTRAIN_REDMOND_API __declspec(dllimport)
#endif /* BUILDING_REDSTRAIN_REDMOND_API */
#else /* !REDSTRAIN_NEEDS_DLLEXPORT */
#define REDSTRAIN_REDMOND_API
#endif

#endif /* REDSTRAIN_MOD_REDMOND_API_HPP */

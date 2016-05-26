#ifndef REDSTRAIN_MOD_CALENDAR_API_HPP
#define REDSTRAIN_MOD_CALENDAR_API_HPP

#include <redstrain/redmond/macros.hpp>

#ifdef REDSTRAIN_NEEDS_DLLEXPORT
#ifdef BUILDING_REDSTRAIN_CALENDAR_API
#define REDSTRAIN_CALENDAR_API __declspec(dllexport)
#else /* !BUILDING_REDSTRAIN_CALENDAR_API */
#define REDSTRAIN_CALENDAR_API __declspec(dllimport)
#endif /* BUILDING_REDSTRAIN_CALENDAR_API */
#else /* !REDSTRAIN_NEEDS_DLLEXPORT */
#define REDSTRAIN_CALENDAR_API
#endif

#endif /* REDSTRAIN_MOD_CALENDAR_API_HPP */
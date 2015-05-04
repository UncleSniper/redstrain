#ifndef REDSTRAIN_MOD_IO_API_HPP
#define REDSTRAIN_MOD_IO_API_HPP

#include <redstrain/redmond/macros.hpp>

#ifdef REDSTRAIN_NEEDS_DLLEXPORT
#ifdef BUILDING_REDSTRAIN_IO_API
#define REDSTRAIN_IO_API __declspec(dllexport)
#else /* !BUILDING_REDSTRAIN_IO_API */
#define REDSTRAIN_IO_API __declspec(dllimport)
#endif /* BUILDING_REDSTRAIN_IO_API */
#else /* !REDSTRAIN_NEEDS_DLLEXPORT */
#define REDSTRAIN_IO_API
#endif

#endif /* REDSTRAIN_MOD_IO_API_HPP */

#ifndef REDSTRAIN_MOD_OGDL_API_HPP
#define REDSTRAIN_MOD_OGDL_API_HPP

#include <redstrain/redmond/macros.hpp>

#ifdef REDSTRAIN_NEEDS_DLLEXPORT
#ifdef BUILDING_REDSTRAIN_OGDL_API
#define REDSTRAIN_OGDL_API __declspec(dllexport)
#else /* !BUILDING_REDSTRAIN_OGDL_API */
#define REDSTRAIN_OGDL_API __declspec(dllimport)
#endif /* BUILDING_REDSTRAIN_OGDL_API */
#else /* !REDSTRAIN_NEEDS_DLLEXPORT */
#define REDSTRAIN_OGDL_API
#endif

#endif /* REDSTRAIN_MOD_OGDL_API_HPP */

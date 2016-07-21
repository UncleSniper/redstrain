#ifndef REDSTRAIN_MOD_TEST_API_HPP
#define REDSTRAIN_MOD_TEST_API_HPP

#include <redstrain/redmond/macros.hpp>

#ifdef REDSTRAIN_NEEDS_DLLEXPORT
#ifdef BUILDING_REDSTRAIN_TEST_API
#define REDSTRAIN_TEST_API __declspec(dllexport)
#else /* !BUILDING_REDSTRAIN_TEST_API */
#define REDSTRAIN_TEST_API __declspec(dllimport)
#endif /* BUILDING_REDSTRAIN_TEST_API */
#else /* !REDSTRAIN_NEEDS_DLLEXPORT */
#define REDSTRAIN_TEST_API
#endif

#endif /* REDSTRAIN_MOD_TEST_API_HPP */

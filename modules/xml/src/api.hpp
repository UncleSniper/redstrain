#ifndef REDSTRAIN_MOD_XML_API_HPP
#define REDSTRAIN_MOD_XML_API_HPP

#include <redstrain/redmond/macros.hpp>

#ifdef REDSTRAIN_NEEDS_DLLEXPORT
#ifdef BUILDING_REDSTRAIN_XML_API
#define REDSTRAIN_XML_API __declspec(dllexport)
#else /* !BUILDING_REDSTRAIN_XML_API */
#define REDSTRAIN_XML_API __declspec(dllimport)
#endif /* BUILDING_REDSTRAIN_XML_API */
#else /* !REDSTRAIN_NEEDS_DLLEXPORT */
#define REDSTRAIN_XML_API
#endif

#endif /* REDSTRAIN_MOD_XML_API_HPP */

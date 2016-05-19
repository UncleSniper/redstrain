#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace calendar {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_CALENDAR)

	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/Calendar, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/Calendar, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/Calendar, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Platform,
			RedStrain/Calendar, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)

}}

#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace platform {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_PLATFORM)

	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/Platform, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/Platform, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/Platform, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)

}}

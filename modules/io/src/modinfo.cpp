#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace io {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_IO)

	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/IO, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/IO, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/IO, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
	REDSTRAIN_DEPEND_MODULE(Platform,
			RedStrain/IO, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)

}}

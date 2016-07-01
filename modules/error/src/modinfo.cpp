#include <redstrain/util/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace error {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_ERROR)

	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/Error, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/Error, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)

}}

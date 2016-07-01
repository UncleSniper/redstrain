#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace algorithm {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_ALGORITHM)

	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/Algorithm, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/Algorithm, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/Algorithm, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)

}}

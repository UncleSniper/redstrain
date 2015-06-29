#include <redstrain/redmond/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace util {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_UTIL)

	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/Util, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)

}}

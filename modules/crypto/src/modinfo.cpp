#include <redstrain/io/modinfo.hpp>
#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace crypto {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_CRYPTO)

	REDSTRAIN_DEPEND_MODULE(IO,
			RedStrain/Crypto, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)
	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/Crypto, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/Crypto, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/Crypto, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
	REDSTRAIN_DEPEND_MODULE(Platform,
			RedStrain/Crypto, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)

}}

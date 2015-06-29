#include <redstrain/io/modinfo.hpp>
#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace protostr {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_PROTOSTR)

	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/ProtoStreams, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/ProtoStreams, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/ProtoStreams, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Platform,
			RedStrain/ProtoStreams, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)
	REDSTRAIN_DEPEND_MODULE(IO,
			RedStrain/ProtoStreams, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)

}}

#include <redstrain/io/modinfo.hpp>
#include <redstrain/util/modinfo.hpp>
#include <redstrain/text/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>
#include <redstrain/protostr/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace locale {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_OGDL)

	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/OGDL, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/OGDL, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/OGDL, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Platform,
			RedStrain/OGDL, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)
	REDSTRAIN_DEPEND_MODULE(IO,
			RedStrain/OGDL, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)
	REDSTRAIN_DEPEND_MODULE(ProtoStreams,
			RedStrain/OGDL, RedStrain/ProtoStreams, REDSTRAIN_MOD_PROTOSTR, ::redengine::protostr)
	REDSTRAIN_DEPEND_MODULE(Text,
			RedStrain/OGDL, RedStrain/Text, REDSTRAIN_MOD_TEXT, ::redengine::text)

}}

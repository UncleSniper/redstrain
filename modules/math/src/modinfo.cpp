#include <redstrain/io/modinfo.hpp>
#include <redstrain/text/modinfo.hpp>
#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>
#include <redstrain/protostr/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace math {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_MATH)

	REDSTRAIN_DEPEND_MODULE(IO,
			RedStrain/Math, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)
	REDSTRAIN_DEPEND_MODULE(Text,
			RedStrain/Math, RedStrain/Text, REDSTRAIN_MOD_TEXT, ::redengine::text)
	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/Math, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/Math, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/Math, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
	REDSTRAIN_DEPEND_MODULE(Platform,
			RedStrain/Math, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)
	REDSTRAIN_DEPEND_MODULE(ProtoStreams,
			RedStrain/Math, RedStrain/ProtoStreams, REDSTRAIN_MOD_PROTOSTR, ::redengine::protostr)

}}

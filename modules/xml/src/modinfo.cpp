#include <redstrain/io/modinfo.hpp>
#include <redstrain/util/modinfo.hpp>
#include <redstrain/text/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>
#include <redstrain/protostr/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace xml {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_XML)

	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/XML, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/XML, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/XML, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Platform,
			RedStrain/XML, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)
	REDSTRAIN_DEPEND_MODULE(IO,
			RedStrain/XML, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)
	REDSTRAIN_DEPEND_MODULE(ProtoStreams,
			RedStrain/XML, RedStrain/ProtoStreams, REDSTRAIN_MOD_PROTOSTR, ::redengine::protostr)
	REDSTRAIN_DEPEND_MODULE(Text,
			RedStrain/XML, RedStrain/Text, REDSTRAIN_MOD_TEXT, ::redengine::text)

}}

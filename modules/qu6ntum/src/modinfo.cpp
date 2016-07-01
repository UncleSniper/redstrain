#include <redstrain/io/modinfo.hpp>
#include <redstrain/log/modinfo.hpp>
#include <redstrain/vfs/modinfo.hpp>
#include <redstrain/text/modinfo.hpp>
#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/locale/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/calendar/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>
#include <redstrain/protostr/modinfo.hpp>
#include <redstrain/algorithm/modinfo.hpp>

#include "modinfo.hpp"

namespace redengine {
namespace qu6ntum {

	REDSTRAIN_DEFINE_MODULE_VERSION(REDSTRAIN_MOD_QU6NTUM)

	REDSTRAIN_DEPEND_MODULE(IO,
			RedStrain/Qu6ntum, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)
	REDSTRAIN_DEPEND_MODULE(Log,
			RedStrain/Qu6ntum, RedStrain/Log, REDSTRAIN_MOD_LOG, ::redengine::log)
	REDSTRAIN_DEPEND_MODULE(VFS,
			RedStrain/Qu6ntum, RedStrain/VFS, REDSTRAIN_MOD_VFS, ::redengine::vfs)
	REDSTRAIN_DEPEND_MODULE(Text,
			RedStrain/Qu6ntum, RedStrain/Text, REDSTRAIN_MOD_TEXT, ::redengine::text)
	REDSTRAIN_DEPEND_MODULE(Util,
			RedStrain/Qu6ntum, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
	REDSTRAIN_DEPEND_MODULE(Error,
			RedStrain/Qu6ntum, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
	REDSTRAIN_DEPEND_MODULE(Locale,
			RedStrain/Qu6ntum, RedStrain/Locale, REDSTRAIN_MOD_LOCALE, ::redengine::locale)
	REDSTRAIN_DEPEND_MODULE(Redmond,
			RedStrain/Qu6ntum, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
	REDSTRAIN_DEPEND_MODULE(Calendar,
			RedStrain/Qu6ntum, RedStrain/Calendar, REDSTRAIN_MOD_CALENDAR, ::redengine::calendar)
	REDSTRAIN_DEPEND_MODULE(Platform,
			RedStrain/Qu6ntum, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)
	REDSTRAIN_DEPEND_MODULE(ProtoStreams,
			RedStrain/Qu6ntum, RedStrain/ProtoStreams, REDSTRAIN_MOD_PROTOSTR, ::redengine::protostr)
	REDSTRAIN_DEPEND_MODULE(Algorithm,
			RedStrain/Qu6ntum, RedStrain/Algorithm, REDSTRAIN_MOD_ALGORITHM, ::redengine::algorithm)

}}

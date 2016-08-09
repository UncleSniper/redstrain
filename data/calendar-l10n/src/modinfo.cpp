#include <redstrain/io/modinfo.hpp>
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
#include <redstrain/redmond/blobs.hpp>

using redengine::redmond::BlobModuleRegistrar;

static BlobModuleRegistrar registerBlobModule("RedStrain/Calendar/L10N");

REDSTRAIN_DEPEND_MODULE(IO,
		RedStrain/Calendar/L10N, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)
REDSTRAIN_DEPEND_MODULE(VFS,
		RedStrain/Calendar/L10N, RedStrain/VFS, REDSTRAIN_MOD_VFS, ::redengine::vfs)
REDSTRAIN_DEPEND_MODULE(Text,
		RedStrain/Calendar/L10N, RedStrain/Text, REDSTRAIN_MOD_TEXT, ::redengine::text)
REDSTRAIN_DEPEND_MODULE(Util,
		RedStrain/Calendar/L10N, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
REDSTRAIN_DEPEND_MODULE(Error,
		RedStrain/Calendar/L10N, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
REDSTRAIN_DEPEND_MODULE(Locale,
		RedStrain/Calendar/L10N, RedStrain/Locale, REDSTRAIN_MOD_LOCALE, ::redengine::locale)
REDSTRAIN_DEPEND_MODULE(Redmond,
		RedStrain/Calendar/L10N, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
REDSTRAIN_DEPEND_MODULE(Calendar,
		RedStrain/Calendar/L10N, RedStrain/Calendar, REDSTRAIN_MOD_CALENDAR, ::redengine::calendar)
REDSTRAIN_DEPEND_MODULE(Platform,
		RedStrain/Calendar/L10N, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)
REDSTRAIN_DEPEND_MODULE(ProtoStreams,
		RedStrain/Calendar/L10N, RedStrain/ProtoStreams, REDSTRAIN_MOD_PROTOSTR, ::redengine::protostr)
REDSTRAIN_DEPEND_MODULE(Algorithm,
		RedStrain/Calendar/L10N, RedStrain/Algorithm, REDSTRAIN_MOD_ALGORITHM, ::redengine::algorithm)
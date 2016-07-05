#include <redstrain/io/modinfo.hpp>
#include <redstrain/vfs/modinfo.hpp>
#include <redstrain/text/modinfo.hpp>
#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/locale/modinfo.hpp>
#include <redstrain/cmdline/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>
#include <redstrain/protostr/modinfo.hpp>
#include <redstrain/algorithm/modinfo.hpp>

REDSTRAIN_DEPEND_MODULE(IO,
		redlnblob, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)
REDSTRAIN_DEPEND_MODULE(VFS,
		redlnblob, RedStrain/VFS, REDSTRAIN_MOD_VFS, ::redengine::vfs)
REDSTRAIN_DEPEND_MODULE(Text,
		redlnblob, RedStrain/Text, REDSTRAIN_MOD_TEXT, ::redengine::text)
REDSTRAIN_DEPEND_MODULE(Util,
		redlnblob, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
REDSTRAIN_DEPEND_MODULE(Error,
		redlnblob, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
REDSTRAIN_DEPEND_MODULE(Locale,
		redlnblob, RedStrain/Locale, REDSTRAIN_MOD_LOCALE, ::redengine::locale)
REDSTRAIN_DEPEND_MODULE(CommandLine,
		redlnblob, RedStrain/CommandLine, REDSTRAIN_MOD_CMDLINE, ::redengine::cmdline)
REDSTRAIN_DEPEND_MODULE(Redmond,
		redlnblob, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
REDSTRAIN_DEPEND_MODULE(Platform,
		redlnblob, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)
REDSTRAIN_DEPEND_MODULE(ProtoStreams,
		redlnblob, RedStrain/ProtoStreams, REDSTRAIN_MOD_PROTOSTR, ::redengine::protostr)
REDSTRAIN_DEPEND_MODULE(Algorithm,
		redlnblob, RedStrain/Algorithm, REDSTRAIN_MOD_ALGORITHM, ::redengine::algorithm)

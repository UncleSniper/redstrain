#include <redstrain/io/modinfo.hpp>
#include <redstrain/util/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/cmdline/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>

REDSTRAIN_DEPEND_MODULE(Redmond,
		redmkblob, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
REDSTRAIN_DEPEND_MODULE(Error,
		redmkblob, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
REDSTRAIN_DEPEND_MODULE(Util,
		redmkblob, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
REDSTRAIN_DEPEND_MODULE(Platform,
		redmkblob, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)
REDSTRAIN_DEPEND_MODULE(IO,
		redmkblob, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)
REDSTRAIN_DEPEND_MODULE(CommandLine,
		redmkblob, RedStrain/CommandLine, REDSTRAIN_MOD_CMDLINE, ::redengine::cmdline)

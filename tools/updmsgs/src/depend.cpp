#include <redstrain/io/modinfo.hpp>
#include <redstrain/util/modinfo.hpp>
#include <redstrain/text/modinfo.hpp>
#include <redstrain/error/modinfo.hpp>
#include <redstrain/redmond/modinfo.hpp>
#include <redstrain/cmdline/modinfo.hpp>
#include <redstrain/platform/modinfo.hpp>
#include <redstrain/protostr/modinfo.hpp>

REDSTRAIN_DEPEND_MODULE(Redmond,
		redupdmsgs, RedStrain/Redmond, REDSTRAIN_MOD_REDMOND, ::redengine::redmond)
REDSTRAIN_DEPEND_MODULE(Error,
		redupdmsgs, RedStrain/Error, REDSTRAIN_MOD_ERROR, ::redengine::error)
REDSTRAIN_DEPEND_MODULE(Util,
		redupdmsgs, RedStrain/Util, REDSTRAIN_MOD_UTIL, ::redengine::util)
REDSTRAIN_DEPEND_MODULE(Platform,
		redupdmsgs, RedStrain/Platform, REDSTRAIN_MOD_PLATFORM, ::redengine::platform)
REDSTRAIN_DEPEND_MODULE(IO,
		redupdmsgs, RedStrain/IO, REDSTRAIN_MOD_IO, ::redengine::io)
REDSTRAIN_DEPEND_MODULE(CommandLine,
		redupdmsgs, RedStrain/CommandLine, REDSTRAIN_MOD_CMDLINE, ::redengine::cmdline)
REDSTRAIN_DEPEND_MODULE(ProtoStreams,
		redupdmsgs, RedStrain/ProtoStreams, REDSTRAIN_MOD_PROTOSTR, ::redengine::protostr)
REDSTRAIN_DEPEND_MODULE(Text,
		redupdmsgs, RedStrain/Text, REDSTRAIN_MOD_TEXT, ::redengine::text)

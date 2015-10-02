#include "XakeGCC.hpp"

using std::string;
using redengine::redmond::Architecture;
using redengine::redmond::OperatingSystem;

namespace redengine {
namespace build {
namespace boot {

	const char *const XakeGCC::DEFAULT_EXECUTABLE = "g++";
	const char *const XakeGCC::DEFAULT_AR_EXECUTABLE = "ar";

	XakeGCC::XakeGCC(const XakeProject& project, const string& executable, const string& arExecutable,
			Architecture architecture, OperatingSystem targetOS) : ExternalTool(executable),
			GCC(executable, arExecutable, architecture, targetOS), project(project) {}

	XakeGCC::XakeGCC(const XakeGCC& gcc) : ExternalTool(gcc), GCC(gcc), project(gcc.project) {}

}}}

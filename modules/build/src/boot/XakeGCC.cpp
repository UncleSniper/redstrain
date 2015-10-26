#include <redstrain/util/Delete.hpp>

#include "XakeGCC.hpp"
#include "XakeUtils.hpp"
#include "Resources.hpp"
#include "XakeProject.hpp"

using std::string;
using redengine::util::Delete;
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

	Compilation* XakeGCC::newCompilation(const string& source, Compilation::CompileMode mode) {
		Delete<Compilation> compilation(GCC::newCompilation(source, mode));
		compilation->setDebug(XakeUtils::parseBoolean(project.getProjectConfiguration()
				.getProperty(Resources::RES_INCLUDE_DEBUG_SYMBOLS)));
		return compilation.set();
	}

	Linkage* XakeGCC::newLinkage(const string& target, Linkage::LinkMode mode) {
		Delete<Linkage> linkage(GCC::newLinkage(target, mode));
		linkage->setDebug(XakeUtils::parseBoolean(project.getProjectConfiguration()
				.getProperty(Resources::RES_INCLUDE_DEBUG_SYMBOLS)));
		return linkage.set();
	}

}}}

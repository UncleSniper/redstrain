#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/redmond/constants.hpp>

#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "../UnsupportedToolchainError.hpp"

using std::string;
using redengine::util::Delete;
using redengine::platform::Stat;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;
using redengine::redmond::Architecture;
using redengine::redmond::OS_LINUX;
using redengine::redmond::OS_WINDOWS;
using redengine::redmond::buildTargetOS;

namespace redengine {
namespace build {
namespace boot {

	// ======== XakeGCC ========

	const char *const XakeProject::XakeGCC::DEFAULT_EXECUTABLE = "g++";
	const char *const XakeProject::XakeGCC::DEFAULT_AR_EXECUTABLE = "ar";

	XakeProject::XakeGCC::XakeGCC(const XakeProject& project, const string& executable, const string& arExecutable,
			Architecture architecture) : ExternalTool(executable), GCC(executable, arExecutable, architecture),
			project(project) {}

	XakeProject::XakeGCC::XakeGCC(const XakeGCC& gcc) : ExternalTool(gcc), GCC(gcc), project(gcc.project) {}

	// ======== XakeProject ========

	XakeProject::XakeProject(const string& baseDirectory) : baseDirectory(baseDirectory),
			compiler(NULL), linker(NULL) {
		configuration.load(Resources::DFL_DEFAULTS);
		switch(buildTargetOS) {
			case OS_LINUX:
				configuration.load(Resources::DFL_LINUX_DEFAULTS);
				break;
			case OS_WINDOWS:
				configuration.load(Resources::DFL_WINDOWS_DEFAULTS);
				break;
			default:
				break;
		}
		Stat stat;
		string pcpath(Pathname::join(baseDirectory, "project.properties"));
		if(Filesystem::access(pcpath, Filesystem::FILE_EXISTS)) {
			Filesystem::stat(pcpath, stat);
			if(stat.getType() == Stat::REGULAR_FILE)
				configuration.load(pcpath);
		}
		string scpath(Pathname::join(baseDirectory, "site.properties"));
		if(Filesystem::access(scpath, Filesystem::FILE_EXISTS)) {
			Filesystem::stat(scpath, stat);
			if(stat.getType() == Stat::REGULAR_FILE)
				configuration.load(scpath);
		}
	}

	XakeProject::XakeProject(const XakeProject& project)
			: baseDirectory(project.baseDirectory), configuration(project.configuration),
			compiler(NULL), linker(NULL) {}

	XakeProject::~XakeProject() {
		ConstComponentIterator begin(components.begin()), end(components.end());
		for(; begin != end; ++begin)
			delete begin->second;
	}

	string XakeProject::getProjectName() const {
		string name(configuration.getProperty(Resources::RES_PROJECT_NAME));
		if(name.empty()) {
			string bn(Pathname::basename(Pathname::tidy(baseDirectory)));
			return bn == Pathname::THIS_DIRECTORY ? "" : bn;
		}
		else
			return name;
	}

	XakeComponent* XakeProject::getComponent(const Component* component) const {
		ConstComponentIterator it = components.find(component);
		return it == components.end() ? NULL : it->second;
	}

	bool XakeProject::addComponent(const Component* abstractComponent, XakeComponent* backingComponent) {
		if(!abstractComponent || !backingComponent)
			return false;
		ComponentIterator it = components.find(abstractComponent);
		if(it == components.end()) {
			components[abstractComponent] = backingComponent;
			return true;
		}
		if(it->second == backingComponent)
			return true;
		return false;
	}

	Compiler* XakeProject::getCompiler() {
		if(!compiler)
			setupCompiler();
		return compiler;
	}


	Linker* XakeProject::getLinker() {
		if(!linker)
			setupCompiler();
		return linker;
	}

	void XakeProject::setupCompiler() {
		string flavor(configuration.getProperty(Resources::RES_COMPILER_FLAVOR));
		if(flavor.empty() || flavor == "gcc")
			setupGCC();
		else
			throw UnsupportedToolchainError(flavor);
	}

	void XakeProject::setupGCC() {
		string executable(configuration.getProperty(Resources::RES_COMPILER_BINARY));
		if(executable.empty())
			executable = XakeGCC::DEFAULT_EXECUTABLE;
		string arExecutable(configuration.getProperty(Resources::RES_STATIC_LINKER_BINARY));
		if(arExecutable.empty())
			arExecutable = XakeGCC::DEFAULT_AR_EXECUTABLE;
		string arch(configuration.getProperty(Resources::RES_RSB_TARGET_ARCHITECTURE));
		Architecture architecture = arch.empty() ? REDSTRAIN_BUILD_DEFAULT_ARCH : Compiler::parseArchitecture(arch);
		Delete<XakeGCC> gcc(new XakeGCC(*this, executable, arExecutable, architecture));
		compiler = *gcc;
		linker = *gcc;
		gcc.set();
	}

}}}

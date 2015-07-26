#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/redmond/constants.hpp>

#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "../ValveGroup.hpp"
#include "../BuildContext.hpp"
#include "../UnsupportedToolchainError.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::platform::Stat;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;
using redengine::redmond::Architecture;
using redengine::redmond::OperatingSystem;
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
			Architecture architecture, OperatingSystem targetOS) : ExternalTool(executable),
			GCC(executable, arExecutable, architecture, targetOS), project(project) {}

	XakeProject::XakeGCC::XakeGCC(const XakeGCC& gcc) : ExternalTool(gcc), GCC(gcc), project(gcc.project) {}

	// ======== XakeCPPLanguage ========

	XakeProject::XakeCPPLanguage::XakeCPPLanguage(XakeProject& project)
			: CPPLanguage(*project.getCompiler()), project(project) {}

	XakeProject::XakeCPPLanguage::XakeCPPLanguage(const XakeCPPLanguage& cpp)
			: CPPLanguage(cpp), project(cpp.project) {}

	CompilerConfiguration& XakeProject::XakeCPPLanguage::getCompilerConfiguration(const Flavor& transformFlavor,
			const Component& component) {
		XakeComponent* xcomponent = project.getComponent(&component);
		if(xcomponent)
			return *xcomponent;
		else
			return CPPLanguage::getCompilerConfiguration(transformFlavor, component);
	}

	// ======== XakeObjectFileLanguage ========

	XakeProject::XakeObjectFileLanguage::XakeObjectFileLanguage(const string& name, XakeProject& project)
			: ObjectFileLanguage(name, *project.getLinker()), project(project) {}

	XakeProject::XakeObjectFileLanguage::XakeObjectFileLanguage(const XakeObjectFileLanguage& language)
			: ObjectFileLanguage(language), project(language.project) {}

	LinkerConfiguration& XakeProject::XakeObjectFileLanguage::getLinkerConfiguration(const Flavor& transformFlavor,
			const Component& component) {
		XakeComponent* xcomponent = project.getComponent(&component);
		if(xcomponent) {
			if(transformFlavor == Flavor::STATIC)
				return xcomponent->getStaticLinkerConfiguration();
			if(transformFlavor == Flavor::DYNAMIC)
				return xcomponent->getDynamicLinkerConfiguration();
		}
		return ObjectFileLanguage::getLinkerConfiguration(transformFlavor, component);
	}

	// ======== XakeProject ========

	XakeProject::XakeProject(const string& baseDirectory) : baseDirectory(baseDirectory),
			compiler(NULL), linker(NULL), cppLanguage(NULL), objectFileLanguage(NULL), cleanValve(NULL),
			buildValve(NULL), modulesValve(NULL), toolsValve(NULL), staticValve(NULL), dynamicValve(NULL) {
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
			compiler(NULL), linker(NULL), cppLanguage(NULL), objectFileLanguage(NULL), cleanValve(NULL),
			buildValve(NULL), modulesValve(NULL), toolsValve(NULL), staticValve(NULL), dynamicValve(NULL) {}

	XakeProject::~XakeProject() {
		ConstComponentIterator cbegin(components.begin()), cend(components.end());
		for(; cbegin != cend; ++cbegin)
			delete cbegin->second;
		if(compiler)
			delete compiler;
		if(cppLanguage)
			delete cppLanguage;
		if(cleanValve)
			cleanValve->unref();
		if(buildValve)
			buildValve->unref();
		if(modulesValve)
			modulesValve->unref();
		if(toolsValve)
			toolsValve->unref();
		if(staticValve)
			staticValve->unref();
		if(dynamicValve)
			dynamicValve->unref();
		ValveIterator vbegin(valves.begin()), vend(valves.end());
		for(; vbegin != vend; ++vbegin)
			vbegin->second->unref();
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
		compilerName = "GCC";
		string executable(configuration.getProperty(Resources::RES_COMPILER_BINARY));
		if(executable.empty())
			executable = XakeGCC::DEFAULT_EXECUTABLE;
		string arExecutable(configuration.getProperty(Resources::RES_STATIC_LINKER_BINARY));
		if(arExecutable.empty())
			arExecutable = XakeGCC::DEFAULT_AR_EXECUTABLE;
		string arch(configuration.getProperty(Resources::RES_RSB_TARGET_ARCHITECTURE));
		Architecture architecture = arch.empty() ? REDSTRAIN_BUILD_DEFAULT_ARCH : Compiler::parseArchitecture(arch);
		string os(configuration.getProperty(Resources::RES_RSB_TARGET_OS));
		OperatingSystem targetOS = os.empty() ? REDSTRAIN_BUILD_DEFAULT_OS : Linker::parseOperatingSystem(os);
		Delete<XakeGCC> gcc(new XakeGCC(*this, executable, arExecutable, architecture, targetOS));
		compiler = *gcc;
		linker = *gcc;
		gcc.set();
	}

	Language* XakeProject::getCPPLanguage() {
		if(!cppLanguage)
			cppLanguage = new XakeCPPLanguage(*this);
		return cppLanguage;
	}

	Language* XakeProject::getObjectFileLanguage() {
		if(!objectFileLanguage)
			objectFileLanguage = new XakeObjectFileLanguage(getLinker()->getObjectFileFormatName(), *this);
		return objectFileLanguage;
	}

	const string& XakeProject::getCompilerName() {
		if(!compiler)
			setupCompiler();
		return compilerName;
	}

	StaticValve* XakeProject::getCleanValve(BuildContext& context) {
		if(!cleanValve)
			cleanValve = &context.getOrMakeValve("clean");
		return cleanValve;
	}

	StaticValve* XakeProject::getBuildValve(BuildContext& context) {
		if(!buildValve) {
			buildValve = &context.getOrMakeValve("build");
			buildValve->ref();
		}
		return buildValve;
	}

	StaticValve* XakeProject::getModulesValve(BuildContext& context) {
		if(!modulesValve) {
			modulesValve = &context.getOrMakeValve("modules");
			modulesValve->ref();
		}
		return modulesValve;
	}

	StaticValve* XakeProject::getToolsValve(BuildContext& context) {
		if(!toolsValve) {
			toolsValve = &context.getOrMakeValve("tools");
			toolsValve->ref();
		}
		return toolsValve;
	}

	StaticValve* XakeProject::getStaticValve(BuildContext& context) {
		if(!staticValve) {
			staticValve = &context.getOrMakeValve("static");
			staticValve->ref();
		}
		return staticValve;
	}

	StaticValve* XakeProject::getDynamicValve(BuildContext& context) {
		if(!dynamicValve) {
			dynamicValve = &context.getOrMakeValve("dynamic");
			dynamicValve->ref();
		}
		return dynamicValve;
	}

	StaticValve* XakeProject::getComponentValve(BuildContext& context, const string& name) {
		ValveIterator it = valves.find(name);
		if(it != valves.end())
			return it->second;
		StaticValve* valve = &context.getOrMakeValve(name);
		valves[name] = valve;
		valve->ref();
		return valve;
	}

	void XakeProject::makeValveGroups(BuildContext& context) {
		Unref<ValveGroup> group;
		StaticValve* dv;
		// build/clean
		group = new ValveGroup;
		dv = getBuildValve(context);
		group->addMember(getCleanValve(context));
		group->addMember(dv);
		group->addDefault(dv);
		context.addValveGroup(*group);
		group.set()->unref();
		// modules/tools
		group = new ValveGroup;
		dv = getModulesValve(context);
		group->addMember(dv);
		group->addDefault(dv);
		dv = getToolsValve(context);
		group->addMember(dv);
		group->addDefault(dv);
		context.addValveGroup(*group);
		group.set()->unref();
		// static/dynamic
		group = new ValveGroup;
		dv = getDynamicValve(context);
		group->addMember(getStaticValve(context));
		group->addMember(dv);
		group->addDefault(dv);
		context.addValveGroup(*group);
		group.set()->unref();
		// components
		group = new ValveGroup;
		ValveIterator vbegin(valves.begin()), vend(valves.end());
		for(; vbegin != vend; ++vbegin) {
			group->addMember(vbegin->second);
			group->addDefault(vbegin->second);
		}
		context.addValveGroup(*group);
		group.set()->unref();
	}

}}}

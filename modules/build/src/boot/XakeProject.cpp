#include <algorithm>
#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Stat.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/redmond/constants.hpp>

#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "../ValveGroup.hpp"
#include "../BuildContext.hpp"
#include "../FileArtifact.hpp"
#include "../UnsupportedToolchainError.hpp"

using std::map;
using std::string;
using std::transform;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::platform::Stat;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;
using redengine::redmond::Architecture;
using redengine::redmond::OperatingSystem;
using redengine::io::CPPArrayOutputStream;
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

	// ======== XakeBlobLanguage ========

	XakeProject::XakeBlobLanguage::XakeBlobLanguage(const XakeProject& project) : project(project) {}

	XakeProject::XakeBlobLanguage::XakeBlobLanguage(const XakeBlobLanguage& language)
			: BlobLanguage(language), project(language.project) {}

	static char slugify(char c) {
		if(
			(c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z')
			|| (c >= '0' && c <= '9')
		)
			return c;
		else
			return '_';
	}

	BlobLanguage::BlobConfiguration* XakeProject::XakeBlobLanguage::getBlobConfiguration(const FileArtifact& source,
			const Flavor&, const Component& component) {
		map<string, string> variables, mvariables;
		variables["project"] = project.getProjectName();
		variables["module"] = component.getName();
		string sourceBasename(Pathname::basename(source.getPathname()));
		string::size_type pos = sourceBasename.rfind('.');
		if(pos != string::npos && pos)
			sourceBasename = sourceBasename.substr(static_cast<string::size_type>(0u), pos);
		transform(sourceBasename.begin(), sourceBasename.end(), sourceBasename.begin(), slugify);
		string ns;
		const XakeComponent* xcomponent = project.getComponent(&component);
		if(xcomponent)
			ns = xcomponent->getComponentConfiguration().getProperty(Resources::RES_RSB_GENERATED_NAMESPACE);
		if(ns.empty())
			ns = project.getProjectConfiguration().getProperty(Resources::RES_RSB_GENERATED_NAMESPACE);
		if(!ns.empty()) {
			ns = XakeUtils::subst(ns, variables);
			if(!StringUtils::endsWith(ns, "::"))
				ns += "::";
		}
		mvariables["project"] = project.getProjectGuard();
		if(xcomponent)
			mvariables["module"] = xcomponent->getComponentGuard();
		else
			mvariables["module"] = XakeUtils::slugifyMacro(component.getName());
		string expmac;
		if(xcomponent)
			expmac = xcomponent->getComponentConfiguration().getProperty(Resources::RES_RSB_EXPORT_MACRO);
		if(expmac.empty())
			expmac = project.getProjectConfiguration().getProperty(Resources::RES_RSB_EXPORT_MACRO);
		if(!expmac.empty())
			expmac = XakeUtils::subst(expmac, mvariables);
		string bpath;
		if(xcomponent)
			bpath = xcomponent->getComponentConfiguration().getProperty(Resources::RES_RSB_BLOB_PATH);
		if(bpath.empty())
			bpath = project.getProjectConfiguration().getProperty(Resources::RES_RSB_BLOB_PATH);
		if(!bpath.empty()) {
			variables["srchead"] = source.getDirectory();
			variables["srctail"] = source.getBasename();
			variables["srcbasename"] = Pathname::basename(source.getPathname());
			variables["srcdirname"] = Pathname::dirname(source.getPathname());
			variables["srcpath"] = source.getPathname();
			bpath = XakeUtils::subst(bpath, variables);
		}
		string xinclude;
		if(xcomponent)
			xinclude = xcomponent->getComponentConfiguration().getProperty(Resources::RES_RSB_BLOB_INCLUDE);
		if(xinclude.empty())
			xinclude = project.getProjectConfiguration().getProperty(Resources::RES_RSB_BLOB_INCLUDE);
		return new XakeBlobConfiguration(ns + sourceBasename, expmac, bpath, xinclude);
	}

	// ======== XakeBlobConfiguration ========

	XakeProject::XakeBlobLanguage::XakeBlobConfiguration::XakeBlobConfiguration(const string& variable,
			const string& exportMacro, const string& blobPath, const string& extraInclude)
			: variable(variable), exportMacro(exportMacro), blobPath(blobPath), extraInclude(extraInclude) {}

	XakeProject::XakeBlobLanguage::XakeBlobConfiguration::XakeBlobConfiguration(const XakeBlobConfiguration&
			configuration) : BlobConfiguration(configuration), variable(configuration.variable),
			exportMacro(configuration.exportMacro), blobPath(configuration.blobPath),
			extraInclude(configuration.extraInclude) {}

	void XakeProject::XakeBlobLanguage::XakeBlobConfiguration::applyConfiguration(CPPArrayOutputStream& stream) {
		stream.setVariableName(variable);
		stream.setExportMacro(exportMacro);
		stream.setBlobPath(blobPath);
		stream.setExtraInclude(extraInclude);
	}

	// ======== XakeProject ========

	XakeProject::XakeProject(const string& baseDirectory) : baseDirectory(baseDirectory),
			compiler(NULL), linker(NULL), cppLanguage(NULL), objectFileLanguage(NULL), codeTableLanguage(NULL),
			blobLanguage(NULL), cleanValve(NULL), buildValve(NULL), modulesValve(NULL), toolsValve(NULL),
			staticValve(NULL), dynamicValve(NULL) {
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
			compiler(NULL), linker(NULL), cppLanguage(NULL), objectFileLanguage(NULL), codeTableLanguage(NULL),
			blobLanguage(NULL), cleanValve(NULL), buildValve(NULL), modulesValve(NULL), toolsValve(NULL),
			staticValve(NULL), dynamicValve(NULL) {}

	XakeProject::~XakeProject() {
		ConstComponentIterator cbegin(components.begin()), cend(components.end());
		for(; cbegin != cend; ++cbegin)
			delete cbegin->second;
		if(compiler)
			delete compiler;
		if(cppLanguage)
			delete cppLanguage;
		if(objectFileLanguage)
			delete objectFileLanguage;
		if(codeTableLanguage)
			delete codeTableLanguage;
		if(blobLanguage)
			delete blobLanguage;
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

	string XakeProject::getProjectGuard() const {
		string guard(configuration.getProperty(Resources::RES_PROJECT_GUARD));
		if(guard.empty())
			guard = XakeUtils::slugifyMacro(getProjectName());
		return guard;
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

	Language* XakeProject::getCodeTableDefinitionLanguage() {
		if(!codeTableLanguage)
			codeTableLanguage = new CodeTableDefinitionLanguage();
		return codeTableLanguage;
	}

	Language* XakeProject::getBlobLanguage() {
		if(!blobLanguage)
			blobLanguage = new XakeBlobLanguage(*this);
		return blobLanguage;
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

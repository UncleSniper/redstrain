#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/redmond/constants.hpp>

#include "XakeGCC.hpp"
#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeCPPLanguage.hpp"
#include "XakeBlobLanguage.hpp"
#include "XakeObjectFileLanguage.hpp"
#include "../UnsupportedToolchainError.hpp"

using std::string;
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

	XakeProject::XakeProject(const string& baseDirectory)
			: baseDirectory(Pathname::tidy(Pathname::join(Pathname::getWorkingDirectory(), baseDirectory))),
			compiler(NULL), linker(NULL), cppLanguage(NULL), objectFileLanguage(NULL), codeTableLanguage(NULL),
			blobLanguage(NULL) {
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

	XakeProject::XakeProject(const XakeProject& project) : configuration(project.configuration), compiler(NULL),
			linker(NULL), cppLanguage(NULL), objectFileLanguage(NULL), codeTableLanguage(NULL),
			blobLanguage(NULL) {}

	XakeProject::~XakeProject() {
		if(compiler)
			delete compiler;
		// 'linker' will always be the same object as 'compiler'
		if(cppLanguage)
			delete cppLanguage;
		if(objectFileLanguage)
			delete objectFileLanguage;
		if(codeTableLanguage)
			delete codeTableLanguage;
		if(blobLanguage)
			delete blobLanguage;
	}

	string XakeProject::getProjectName() const {
		string name(configuration.getProperty(Resources::RES_PROJECT_NAME));
		return name.empty() ? Pathname::basename(baseDirectory) : name;
	}

	string XakeProject::getProjectGuard() const {
		string guard(configuration.getProperty(Resources::RES_PROJECT_GUARD));
		return guard.empty() ? XakeUtils::slugifyMacro(getProjectName()) : guard;
	}

	Compiler& XakeProject::getCompiler() {
		if(!compiler)
			setupCompiler();
		return *compiler;
	}

	Linker& XakeProject::getLinker() {
		if(!linker)
			setupCompiler();
		return *linker;
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

	Language& XakeProject::getCPPLanguage() {
		if(!cppLanguage)
			cppLanguage = new XakeCPPLanguage(*this);
		return *cppLanguage;
	}

	Language& XakeProject::getObjectFileLanguage() {
		if(!objectFileLanguage)
			objectFileLanguage = new XakeObjectFileLanguage(*this);
		return *objectFileLanguage;
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

}}}

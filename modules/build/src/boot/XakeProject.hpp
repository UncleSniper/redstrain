#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP

#include "Resources.hpp"
#include "../Linker.hpp"
#include "../Compiler.hpp"
#include "../Language.hpp"
#include "../CodeTableDefinitionLanguage.hpp"
#include "../Messages16DefinitionLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeCPPLanguage;
	class XakeBlobLanguage;
	class XakeBlobAliasLanguage;
	class XakeObjectFileLanguage;
	class XakeMessageBlobAliasLanguage;
	class XakeCodeTable16RegisterLanguage;
	class XakeMessageBlobRegisterLanguage;

	class REDSTRAIN_BUILD_API XakeProject {

	  private:
		const std::string baseDirectory;
		Resources configuration;
		Compiler* compiler;
		Linker* linker;
		XakeCPPLanguage* cppLanguage;
		XakeObjectFileLanguage* objectFileLanguage;
		CodeTableDefinitionLanguage* codeTableLanguage;
		XakeBlobLanguage* blobLanguage;
		XakeBlobAliasLanguage* blobAliasLanguage;
		XakeCodeTable16RegisterLanguage* ct16RegisterLanguage;
		Messages16DefinitionLanguage* messages16Language;
		XakeMessageBlobRegisterLanguage* messageBlobRegisterLanguage;
		XakeMessageBlobAliasLanguage* messageBlobAliasLanguage;
		std::string compilerName;

	  private:
		void setupCompiler();
		void setupGCC();

	  public:
		XakeProject(const std::string&);
		XakeProject(const XakeProject&);
		virtual ~XakeProject();

		inline const std::string& getBaseDirectory() const {
			return baseDirectory;
		}

		inline Resources& getProjectConfiguration() {
			return configuration;
		}

		inline const Resources& getProjectConfiguration() const {
			return configuration;
		}

		std::string getProjectName() const;
		std::string getProjectGuard() const;

		Compiler& getCompiler();
		Linker& getLinker();
		Language& getCPPLanguage();
		Language& getObjectFileLanguage();
		Language& getCodeTableDefinitionLanguage();
		Language& getBlobLanguage();
		Language& getBlobAliasLanguage();
		Language& getCodeTable16RegisterLanguage();
		Language& getMessages16DefinitionLanguage();
		Language& getMessageBlobRegisterLanguage();
		Language& getMessageBlobAliasLanguage();
		const std::string& getCompilerName();

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP */

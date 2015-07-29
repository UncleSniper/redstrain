#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP

#include <map>

#include "../GCC.hpp"
#include "Resources.hpp"
#include "../Component.hpp"
#include "../CPPLanguage.hpp"
#include "../StaticValve.hpp"
#include "../BlobLanguage.hpp"
#include "../ObjectFileLanguage.hpp"
#include "../CodeTableDefinitionLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeComponent;

	class REDSTRAIN_BUILD_API XakeProject {

	  public:
		class REDSTRAIN_BUILD_API XakeGCC : public GCC {

		  public:
			static const char *const DEFAULT_EXECUTABLE;
			static const char *const DEFAULT_AR_EXECUTABLE;

		  private:
			const XakeProject& project;

		  public:
			XakeGCC(const XakeProject&, const std::string&, const std::string&,
					redmond::Architecture, redmond::OperatingSystem);
			XakeGCC(const XakeGCC&);

			inline const XakeProject& getProject() const {
				return project;
			}

		};

		class REDSTRAIN_BUILD_API XakeCPPLanguage : public CPPLanguage {

		  private:
			const XakeProject& project;

		  protected:
			virtual CompilerConfiguration& getCompilerConfiguration(const Flavor&, const Component&);

		  public:
			XakeCPPLanguage(XakeProject&);
			XakeCPPLanguage(const XakeCPPLanguage&);

			inline const XakeProject& getProject() const {
				return project;
			}

		};

		class REDSTRAIN_BUILD_API XakeObjectFileLanguage : public ObjectFileLanguage {

		  private:
			const XakeProject& project;

		  protected:
			virtual LinkerConfiguration& getLinkerConfiguration(const Flavor&, const Component&);

		  public:
			XakeObjectFileLanguage(const std::string&, XakeProject&);
			XakeObjectFileLanguage(const XakeObjectFileLanguage&);

			inline const XakeProject& getProject() const {
				return project;
			}

		};

		class REDSTRAIN_BUILD_API XakeBlobLanguage : public BlobLanguage {

		  public:
			class REDSTRAIN_BUILD_API XakeBlobConfiguration : public BlobConfiguration {

			  private:
				const std::string variable, exportMacro, blobPath;

			  public:
				XakeBlobConfiguration(const std::string&, const std::string&, const std::string&);
				XakeBlobConfiguration(const XakeBlobConfiguration&);

				inline const std::string& getVariableName() const {
					return variable;
				}

				inline const std::string& getExportMacro() const {
					return exportMacro;
				}

				inline const std::string& getBlobPath() const {
					return blobPath;
				}

				virtual void applyConfiguration(io::CPPArrayOutputStream&);

			};

		  private:
			const XakeProject& project;

		  protected:
			virtual BlobConfiguration* getBlobConfiguration(const FileArtifact&, const Flavor&, const Component&);

		  public:
			XakeBlobLanguage(const XakeProject&);
			XakeBlobLanguage(const XakeBlobLanguage&);

			inline const XakeProject& getProject() const {
				return project;
			}

		};

	  private:
		typedef std::map<const Component*, XakeComponent*> Components;
		typedef Components::iterator ComponentIterator;
		typedef Components::const_iterator ConstComponentIterator;
		typedef std::map<std::string, StaticValve*> Valves;
		typedef Valves::const_iterator ValveIterator;

	  private:
		const std::string baseDirectory;
		Resources configuration;
		Components components;
		Compiler* compiler;
		Linker* linker;
		XakeCPPLanguage* cppLanguage;
		XakeObjectFileLanguage* objectFileLanguage;
		CodeTableDefinitionLanguage* codeTableLanguage;
		XakeBlobLanguage* blobLanguage;
		std::string compilerName;
		StaticValve *cleanValve, *buildValve, *modulesValve, *toolsValve, *staticValve, *dynamicValve;
		Valves valves;

	  private:
		void setupCompiler();
		void setupGCC();

	  public:
		XakeProject(const std::string&);
		XakeProject(const XakeProject&);
		~XakeProject();

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

		XakeComponent* getComponent(const Component*) const;
		bool addComponent(const Component*, XakeComponent*);

		Compiler* getCompiler();
		Linker* getLinker();
		Language* getCPPLanguage();
		Language* getObjectFileLanguage();
		Language* getCodeTableDefinitionLanguage();
		Language* getBlobLanguage();
		const std::string& getCompilerName();

		StaticValve* getCleanValve(BuildContext&);
		StaticValve* getBuildValve(BuildContext&);
		StaticValve* getModulesValve(BuildContext&);
		StaticValve* getToolsValve(BuildContext&);
		StaticValve* getStaticValve(BuildContext&);
		StaticValve* getDynamicValve(BuildContext&);
		StaticValve* getComponentValve(BuildContext&, const std::string&);
		void makeValveGroups(BuildContext&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP */

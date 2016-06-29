#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENT_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENT_HPP

#include "Resources.hpp"
#include "../Component.hpp"
#include "../ArtifactStage.hpp"
#include "XakeLinkerConfiguration.hpp"
#include "XakeCompilerConfiguration.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeComponent : public Component {

	  public:
		static const char *const DEFAULT_COMMON_MODULES_PROPERTIES_FILE;
		static const char *const DEFAULT_COMMON_TOOLS_PROPERTIES_FILE;
		static const char *const DEFAULT_COMPONENT_PROPERTIES_FILE;

	  private:
		XakeProject& project;
		Resources configuration;
		XakeCompilerConfiguration compilerConfiguration;
		XakeLinkerConfiguration staticLinkerConfiguration, dynamicLinkerConfiguration;
		ArtifactStage artifactStage;

	  public:
		XakeComponent(XakeProject&, const std::string&, Component::Type);
		XakeComponent(const XakeComponent&);

		inline XakeProject& getProject() {
			return project;
		}

		inline const XakeProject& getProject() const {
			return project;
		}

		inline Resources& getComponentConfiguration() {
			return configuration;
		}

		inline const Resources& getComponentConfiguration() const {
			return configuration;
		}

		inline CompilerConfiguration& getCompilerConfiguration() const {
			return const_cast<XakeCompilerConfiguration&>(compilerConfiguration);
		}

		inline LinkerConfiguration& getStaticLinkerConfiguration() const {
			return const_cast<XakeLinkerConfiguration&>(staticLinkerConfiguration);
		}

		inline LinkerConfiguration& getDynamicLinkerConfiguration() const {
			return const_cast<XakeLinkerConfiguration&>(dynamicLinkerConfiguration);
		}

		inline ArtifactStage& getArtifactStage() const {
			return const_cast<ArtifactStage&>(artifactStage);
		}

		std::string getComponentGuard() const;
		std::string getAPIBuildMacro() const;
		std::string getPresenceMacro() const;

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENT_HPP */

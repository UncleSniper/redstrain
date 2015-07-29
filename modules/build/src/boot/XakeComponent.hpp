#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENT_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENT_HPP

#include "Resources.hpp"
#include "../Component.hpp"
#include "../LinkerConfiguration.hpp"
#include "../CompilerConfiguration.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeComponent : public CompilerConfiguration {

	  public:
		class REDSTRAIN_BUILD_API XakeLinkerConfiguration : public LinkerConfiguration {

		  private:
			XakeComponent& component;
			bool forStatic;

		  public:
			XakeLinkerConfiguration(XakeComponent&, bool);
			XakeLinkerConfiguration(const XakeLinkerConfiguration&);

			virtual void applyConfiguration(Linkage&);

		};

	  public:
		static const char *const DEFAULT_COMMON_MODULES_PROPERTIES_FILE;
		static const char *const DEFAULT_COMMON_TOOLS_PROPERTIES_FILE;
		static const char *const DEFAULT_COMPONENT_PROPERTIES_FILE;

	  private:
		XakeProject& project;
		const std::string baseDirectory;
		const Component::Type type;
		Resources configuration;
		Component* component;
		XakeLinkerConfiguration staticLinkerConfiguration, dynamicLinkerConfiguration;

	  public:
		XakeComponent(XakeProject&, const std::string&, Component::Type);
		XakeComponent(const XakeComponent&);

		inline XakeProject& getProject() {
			return project;
		}

		inline const XakeProject& getProject() const {
			return project;
		}

		inline const std::string& getBaseDirectory() const {
			return baseDirectory;
		}

		inline Component::Type getComponentType() const {
			return type;
		}

		inline Resources& getComponentConfiguration() {
			return configuration;
		}

		inline const Resources& getComponentConfiguration() const {
			return configuration;
		}

		inline Component* getComponent() const {
			return component;
		}

		inline void setComponent(Component* component) {
			this->component = component;
		}

		inline LinkerConfiguration& getStaticLinkerConfiguration() {
			return staticLinkerConfiguration;
		}

		inline const LinkerConfiguration& getStaticLinkerConfiguration() const {
			return staticLinkerConfiguration;
		}

		inline LinkerConfiguration& getDynamicLinkerConfiguration() {
			return dynamicLinkerConfiguration;
		}

		inline const LinkerConfiguration& getDynamicLinkerConfiguration() const {
			return dynamicLinkerConfiguration;
		}

		std::string getComponentGuard() const;

		virtual void applyConfiguration(Compilation&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENT_HPP */

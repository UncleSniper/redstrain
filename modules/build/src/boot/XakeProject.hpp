#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP

#include <map>

#include "../GCC.hpp"
#include "Resources.hpp"
#include "../Component.hpp"

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
			XakeGCC(const XakeProject&, const std::string&, const std::string&, redmond::Architecture);
			XakeGCC(const XakeGCC&);

			inline const XakeProject& getProject() const {
				return project;
			}

		};

	  private:
		typedef std::map<const Component*, XakeComponent*> Components;
		typedef Components::iterator ComponentIterator;
		typedef Components::const_iterator ConstComponentIterator;

	  private:
		const std::string baseDirectory;
		Resources configuration;
		Components components;
		Compiler* compiler;
		Linker* linker;

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

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP */

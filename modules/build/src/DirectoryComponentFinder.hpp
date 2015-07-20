#ifndef REDSTRAIN_MOD_BUILD_DIRECTORYCOMPONENTFINDER_HPP
#define REDSTRAIN_MOD_BUILD_DIRECTORYCOMPONENTFINDER_HPP

#include <map>

#include "ProjectBuilder.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API DirectoryComponentFinder : public ProjectBuilder::ComponentFinder {

	  private:
		typedef std::map<std::string, Component::Type> Directories;
		typedef Directories::iterator DirectoriesIterator;
		typedef Directories::const_iterator ConstDirectoriesIterator;

	  private:
		Directories directories;

	  public:
		DirectoryComponentFinder();
		DirectoryComponentFinder(const DirectoryComponentFinder&);

		bool addComponentsDirectory(const std::string&, Component::Type);
		bool removeComponentsDirectory(const std::string&);
		void clearComponentsDirectories();

		virtual void findComponents(const Project&, ProjectBuilder&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DIRECTORYCOMPONENTFINDER_HPP */

#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "Project.hpp"
#include "DirectoryComponentFinder.hpp"

using std::string;
using redengine::util::Appender;
using redengine::platform::Stat;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;

namespace redengine {
namespace build {

	DirectoryComponentFinder::DirectoryComponentFinder() {}

	DirectoryComponentFinder::DirectoryComponentFinder(const DirectoryComponentFinder& finder)
			: ComponentFinder(finder), directories(finder.directories) {}

	bool DirectoryComponentFinder::addComponentsDirectory(const string& directory, Component::Type type) {
		DirectoriesIterator it = directories.find(directory);
		if(it == directories.end()) {
			directories[directory] = type;
			return true;
		}
		if(it->second == type)
			return false;
		it->second = type;
		return true;
	}

	bool DirectoryComponentFinder::removeComponentsDirectory(const string& directory) {
		DirectoriesIterator it = directories.find(directory);
		if(it == directories.end())
			return false;
		directories.erase(it);
		return true;
	}

	void DirectoryComponentFinder::clearComponentsDirectories() {
		directories.clear();
	}

	struct ComponentDirectoryAppender : Appender<string> {

		const string& metaDirectory;
		ProjectBuilder& builder;
		Component::Type type;

		ComponentDirectoryAppender(const string& metaDirectory, ProjectBuilder& builder, Component::Type type)
				: metaDirectory(metaDirectory), builder(builder), type(type) {}

		virtual void append(const string& child) {
			string cdir(Pathname::join(metaDirectory, child));
			Stat stat;
			Filesystem::stat(cdir, stat);
			if(stat.getType() == Stat::DIRECTORY)
				builder.foundComponent(cdir, type);
		}

	};

	void DirectoryComponentFinder::findComponents(const Project& project, ProjectBuilder& builder) {
		const string& pbd = project.getBaseDirectory();
		ConstDirectoriesIterator dbegin(directories.begin()), dend(directories.end());
		for(; dbegin != dend; ++dbegin) {
			string mdir(Pathname::join(pbd, dbegin->first));
			if(!Filesystem::access(mdir, Filesystem::FILE_EXISTS))
				continue;
			Stat stat;
			Filesystem::stat(mdir, stat);
			if(stat.getType() != Stat::DIRECTORY)
				continue;
			ComponentDirectoryAppender sink(mdir, builder, dbegin->second);
			Filesystem::readdir(mdir, sink);
		}
	}

}}

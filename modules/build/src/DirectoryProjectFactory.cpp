#include <redstrain/platform/Pathname.hpp>

#include "Project.hpp"
#include "DirectoryProjectFactory.hpp"

using std::string;
using redengine::platform::Pathname;

namespace redengine {
namespace build {

	const char *const DirectoryProjectFactory::DEFAULT_NAME = "unnamed";

	DirectoryProjectFactory::DirectoryProjectFactory(const string& defaultName) : defaultName(defaultName) {
		if(defaultName.empty())
			this->defaultName = DirectoryProjectFactory::DEFAULT_NAME;
	}

	DirectoryProjectFactory::DirectoryProjectFactory(const DirectoryProjectFactory& factory)
			: ProjectFactory(factory), defaultName(factory.defaultName) {}

	void DirectoryProjectFactory::setDefaultName(const string& name) {
		if(name.empty())
			defaultName = DirectoryProjectFactory::DEFAULT_NAME;
		else
			defaultName = name;
	}

	Project* DirectoryProjectFactory::newProject(const string& baseDirectory) {
		string bn(Pathname::basename(Pathname::tidy(baseDirectory)));
		if(bn.empty() || bn == Pathname::THIS_DIRECTORY)
			bn = defaultName;
		return new Project(bn, baseDirectory);
	}

}}

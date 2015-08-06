#include "Project.hpp"
#include "StaticProjectFactory.hpp"

using std::string;

namespace redengine {
namespace build {

	StaticProjectFactory::StaticProjectFactory(const string& name) : name(name) {}

	StaticProjectFactory::StaticProjectFactory(const StaticProjectFactory& factory)
			: ProjectFactory(factory), name(factory.name) {}

	void StaticProjectFactory::setName(const string& name) {
		this->name = name;
	}

	Project* StaticProjectFactory::newProject(const string& baseDirectory) {
		return new Project(name, baseDirectory);
	}

}}

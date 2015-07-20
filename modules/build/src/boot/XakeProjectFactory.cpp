#include "../Project.hpp"
#include "XakeProject.hpp"
#include "XakeProjectFactory.hpp"

using std::string;

namespace redengine {
namespace build {
namespace boot {

	XakeProjectFactory::XakeProjectFactory(const XakeProject& project) : project(project) {}

	XakeProjectFactory::XakeProjectFactory(const XakeProjectFactory& factory)
			: DirectoryProjectFactory(factory), project(factory.project) {}

	Project* XakeProjectFactory::newProject(const string& baseDirectory) {
		string name(project.getProjectConfiguration().getProperty(Resources::RES_PROJECT_NAME));
		if(name.empty())
			return DirectoryProjectFactory::newProject(baseDirectory);
		else
			return new Project(name, baseDirectory);
	}

}}}

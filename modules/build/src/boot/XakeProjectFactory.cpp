#include "../Project.hpp"
#include "XakeProject.hpp"
#include "XakeProjectFactory.hpp"

using std::string;

namespace redengine {
namespace build {
namespace boot {

	XakeProjectFactory::XakeProjectFactory(const XakeProject& project) : project(project) {}

	XakeProjectFactory::XakeProjectFactory(const XakeProjectFactory& factory)
			: ProjectFactory(factory), project(factory.project) {}

	Project* XakeProjectFactory::newProject(const string&) {
		return new Project(project.getProjectName(), project.getBaseDirectory());
	}

}}}

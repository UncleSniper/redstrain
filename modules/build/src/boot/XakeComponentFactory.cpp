#include <redstrain/util/Delete.hpp>

#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeComponentFactory.hpp"

using std::string;
using redengine::util::Delete;

namespace redengine {
namespace build {
namespace boot {

	XakeComponentFactory::XakeComponentFactory(XakeProject& project) : project(project) {}

	XakeComponentFactory::XakeComponentFactory(const XakeComponentFactory& factory)
			: ComponentFactory(factory), project(factory.project) {}

	Component* XakeComponentFactory::newComponent(const Project&,
			const string& baseDirectory, Component::Type type) {
		Delete<XakeComponent> component(new XakeComponent(project, baseDirectory, type));
		component->addSourceDirectory(project.getProjectConfiguration().getProperty(Resources::RES_SOURCE_DIRECTORY));
		component->addLanguage(project.getCPPLanguage());
		component->addLanguage(project.getObjectFileLanguage());
		switch(type) {
			case Component::BLOB:
				component->addLanguage(project.getBlobLanguage());
				component->addLanguage(project.getBlobAliasLanguage());
				component->addLanguage(project.getCodeTable16RegisterLanguage());
			case Component::DATA:
				component->addLanguage(project.getCodeTableDefinitionLanguage());
				break;
			default:
				break;
		}
		return component.set();
	}

}}}

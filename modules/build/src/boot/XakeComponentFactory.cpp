#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeComponentFactory.hpp"

using std::string;
using redengine::util::Delete;
using redengine::platform::Pathname;

namespace redengine {
namespace build {
namespace boot {

	XakeComponentFactory::XakeComponentFactory(XakeProject& project) : project(project) {}

	XakeComponentFactory::XakeComponentFactory(const XakeComponentFactory& factory)
			: ComponentFactory(factory), project(factory.project) {}

	Component* XakeComponentFactory::newComponent(const Project&,
			const string& baseDirectory, Component::Type type) {
		Delete<XakeComponent> xcomponent(new XakeComponent(project, baseDirectory, type));
		string name(xcomponent->getComponentConfiguration().getProperty(Resources::RES_COMPONENT_NAME));
		if(name.empty())
			name = Pathname::basename(Pathname::tidy(baseDirectory));
		Delete<Component> component(new Component(type, name, baseDirectory));
		setupComponent(**component, **xcomponent);
		if(project.addComponent(*component, *xcomponent))
			xcomponent.set();
		return component.set();
	}

	void XakeComponentFactory::setupComponent(Component&, const XakeComponent&) const {
		//TODO
	}

}}}

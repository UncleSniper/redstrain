#include "Project.hpp"
#include "Component.hpp"

using std::map;
using std::string;

namespace redengine {
namespace build {

	Project::Project() {}

	Project::Project(const Project& project) : components(project.components) {
		ComponentIterator begin(components.begin()), end(components.end());
		for(; begin != end; ++begin)
			begin->second->ref();
	}

	Project::~Project() {
		ComponentIterator begin(components.begin()), end(components.end());
		for(; begin != end; ++begin)
			begin->second->unref();
	}

	bool Project::addComponent(Component* component) {
		if(!component)
			return false;
		const string& name = component->getName();
		ComponentIterator it = components.find(name);
		if(it != components.end()) {
			Component* old = it->second;
			if(old == component)
				return false;
			components[name] = component;
			component->ref();
			old->unref();
		}
		else {
			components[name] = component;
			component->ref();
		}
		return true;
	}

	bool Project::removeComponent(Component* component) {
		if(!component)
			return false;
		const string& name = component->getName();
		ComponentIterator it = components.find(name);
		if(it == components.end() || it->second != component)
			return false;
		components.erase(it);
		component->ref();
		return true;
	}

	void Project::clearComponents() {
		ComponentIterator begin(components.begin()), end(components.end());
		for(; begin != end; ++begin)
			begin->second->unref();
		components.clear();
	}

	Component* Project::getComponent(const string& name) const {
		map<string, Component*>::const_iterator it = components.find(name);
		return it == components.end() ? NULL : it->second;
	}

}}

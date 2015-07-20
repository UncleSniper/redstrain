#include "Project.hpp"
#include "Component.hpp"

using std::map;
using std::string;

namespace redengine {
namespace build {

	// ======== ComponentNameIterator ========

	Project::ComponentNameIterator::ComponentNameIterator() {}

	Project::ComponentNameIterator::ComponentNameIterator(const Components::const_iterator& iterator)
			: iterator(iterator) {}

	Project::ComponentNameIterator::ComponentNameIterator(const ComponentNameIterator& iterator)
			: iterator(iterator.iterator) {}

	const string& Project::ComponentNameIterator::operator*() const {
		return iterator->first;
	}

	const string* Project::ComponentNameIterator::operator->() const {
		return &iterator->first;
	}

	Project::ComponentNameIterator& Project::ComponentNameIterator::operator++() {
		++iterator;
		return *this;
	}

	Project::ComponentNameIterator Project::ComponentNameIterator::operator++(int) {
		Components::const_iterator old(iterator);
		++iterator;
		return ComponentNameIterator(old);
	}

	bool Project::ComponentNameIterator::operator==(const ComponentNameIterator& other) const {
		return iterator == other.iterator;
	}

	bool Project::ComponentNameIterator::operator!=(const ComponentNameIterator& other) const {
		return iterator != other.iterator;
	}

	Project::ComponentNameIterator& Project::ComponentNameIterator::operator=(const ComponentNameIterator& other) {
		iterator = other.iterator;
		return *this;
	}

	// ======== Project ========

	Project::Project(const string& name, const string& baseDirectory)
			: name(name), baseDirectory(baseDirectory) {}

	Project::Project(const Project& project)
			: name(project.name), baseDirectory(project.baseDirectory), components(project.components) {
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

	void Project::getComponents(ComponentNameIterator& begin, ComponentNameIterator& end) const {
		begin = ComponentNameIterator(components.begin());
		end = ComponentNameIterator(components.end());
	}

}}

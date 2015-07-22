#include "Project.hpp"
#include "Component.hpp"

using std::map;
using std::string;

namespace redengine {
namespace build {

	// ======== ComponentReference ========

	Project::ComponentReference::ComponentReference(ComponentCategory category, const string& name)
			: category(category), name(name) {}

	Project::ComponentReference::ComponentReference(const ComponentReference& reference)
			: category(reference.category), name(reference.name) {}

	bool Project::ComponentReference::operator==(const ComponentReference& reference) const {
		return category == reference.category && name == reference.name;
	}

	bool Project::ComponentReference::operator!=(const ComponentReference& reference) const {
		return category != reference.category || name != reference.name;
	}

	bool Project::ComponentReference::operator<(const ComponentReference& reference) const {
		if(category < reference.category)
			return true;
		if(category > reference.category)
			return false;
		return name < reference.name;
	}

	// ======== ComponentNameIterator ========

	Project::ComponentNameIterator::ComponentNameIterator() {}

	Project::ComponentNameIterator::ComponentNameIterator(const Components::const_iterator& iterator)
			: iterator(iterator) {}

	Project::ComponentNameIterator::ComponentNameIterator(const ComponentNameIterator& iterator)
			: iterator(iterator.iterator) {}

	const Project::ComponentReference& Project::ComponentNameIterator::operator*() const {
		return iterator->first;
	}

	const Project::ComponentReference* Project::ComponentNameIterator::operator->() const {
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
		ComponentReference ref(component->getType() == Component::EXECUTABLE ? EXECUTABLE : LIBRARY,
				component->getName());
		ComponentIterator it = components.find(ref);
		if(it != components.end()) {
			Component* old = it->second;
			if(old == component)
				return false;
			components[ref] = component;
			component->ref();
			old->unref();
		}
		else {
			components[ref] = component;
			component->ref();
		}
		return true;
	}

	bool Project::removeComponent(Component* component) {
		if(!component)
			return false;
		ComponentReference ref(component->getType() == Component::EXECUTABLE ? EXECUTABLE : LIBRARY,
				component->getName());
		ComponentIterator it = components.find(ref);
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

	Component* Project::getComponent(ComponentCategory category, const string& name) const {
		map<ComponentReference, Component*>::const_iterator it = components.find(ComponentReference(category, name));
		return it == components.end() ? NULL : it->second;
	}

	Component* Project::getComponent(const ComponentReference& reference) const {
		map<ComponentReference, Component*>::const_iterator it = components.find(reference);
		return it == components.end() ? NULL : it->second;
	}

	void Project::getComponents(ComponentNameIterator& begin, ComponentNameIterator& end) const {
		begin = ComponentNameIterator(components.begin());
		end = ComponentNameIterator(components.end());
	}

}}

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

	// ======== Project ========

	Project::Project(const string& name, const string& baseDirectory)
			: name(name), baseDirectory(baseDirectory) {}

	Project::Project(const Project& project)
			: name(project.name), baseDirectory(project.baseDirectory), components(project.components) {
		ConstComponentIterator begin(components.begin()), end(components.end());
		for(; begin != end; ++begin)
			begin->second->ref();
	}

	Project::~Project() {
		ConstComponentIterator begin(components.begin()), end(components.end());
		for(; begin != end; ++begin)
			begin->second->unref();
	}

	bool Project::addComponent(Component& component) {
		ComponentReference ref(component.getType() == Component::EXECUTABLE ? EXECUTABLE : LIBRARY,
				component.getName());
		ComponentIterator it = components.find(ref);
		if(it != components.end()) {
			Component* old = it->second;
			if(old == &component)
				return false;
			it->second = &component;
			component.ref();
			old->unref();
		}
		else {
			components[ref] = &component;
			component.ref();
		}
		return true;
	}

	bool Project::removeComponent(Component& component) {
		ComponentReference ref(component.getType() == Component::EXECUTABLE ? EXECUTABLE : LIBRARY,
				component.getName());
		ComponentIterator it = components.find(ref);
		if(it == components.end() || it->second != &component)
			return false;
		components.erase(it);
		component.unref();
		return true;
	}

	void Project::clearComponents() {
		ConstComponentIterator begin(components.begin()), end(components.end());
		for(; begin != end; ++begin)
			begin->second->unref();
		components.clear();
	}

	Component* Project::getComponent(ComponentCategory category, const string& name) const {
		ConstComponentIterator it = components.find(ComponentReference(category, name));
		return it == components.end() ? NULL : it->second;
	}

	Component* Project::getComponent(const ComponentReference& reference) const {
		ConstComponentIterator it = components.find(reference);
		return it == components.end() ? NULL : it->second;
	}

	void Project::getComponents(ComponentNameIterator& begin, ComponentNameIterator& end) const {
		begin = ComponentNameIterator(components.begin());
		end = ComponentNameIterator(components.end());
	}

	string::size_type Project::getMaximalComponentNameWidth() const {
		string::size_type max = static_cast<string::size_type>(0u);
		ConstComponentIterator begin(components.begin()), end(components.end());
		for(; begin != end; ++begin) {
			string::size_type cur = begin->second->getName().length();
			if(cur > max)
				max = cur;
		}
		return max;
	}

}}

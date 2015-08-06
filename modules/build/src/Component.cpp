#include "Component.hpp"

using std::set;
using std::list;
using std::string;

namespace redengine {
namespace build {

	Component::Component(Type type, const string& name, const string& baseDirectory)
			: type(type), name(name), baseDirectory(baseDirectory) {}

	Component::Component(const Component& component)
			: ReferenceCounted(component), type(component.type), name(component.name),
			baseDirectory(component.baseDirectory), sourceDirectories(component.sourceDirectories) {}

	bool Component::addSourceDirectory(const string& directory) {
		return sourceDirectories.append(directory);
	}

	bool Component::removeSourceDirecotry(const string& directory) {
		return sourceDirectories.erase(directory);
	}

	void Component::clearSourceDirectories() {
		sourceDirectories.clear();
	}

	void Component::getSourceDirectories(PathIterator& begin, PathIterator& end) const {
		begin = sourceDirectories.begin();
		end = sourceDirectories.end();
	}

	bool Component::addLanguage(Language& language) {
		return languages.insert(&language).second;
	}

	bool Component::removeLanguage(Language& language) {
		return !!languages.erase(&language);
	}

	void Component::clearLanguages() {
		languages.clear();
	}

	void Component::getLanguages(LanguageIterator& begin, LanguageIterator& end) const {
		begin = languages.begin();
		end = languages.end();
	}

	bool Component::addDependency(Component& component) {
		if(!dependencies.insert(&component).second)
			return false;
		component.ref();
		return true;
	}

	bool Component::removeDependency(Component& component) {
		if(!dependencies.erase(&component))
			return false;
		component.unref();
		return true;
	}

	void Component::clearDependencies() {
		ComponentIterator depbegin(dependencies.begin()), depend(dependencies.end());
		for(; depbegin != depend; ++depbegin)
			(*depbegin)->unref();
		dependencies.clear();
	}

	void Component::getDependencies(ComponentIterator& begin, ComponentIterator& end) const {
		begin = dependencies.begin();
		end = dependencies.end();
	}

	static void getTransitiveDependenciesImpl(const Component& component, list<Component*>& deplist,
			set<Component*>& depset) {
		Component::ComponentIterator ddbegin, ddend;
		component.getDependencies(ddbegin, ddend);
		for(; ddbegin != ddend; ++ddbegin)
			getTransitiveDependenciesImpl(**ddbegin, deplist, depset);
		component.getDependencies(ddbegin, ddend);
		for(; ddbegin != ddend; ++ddbegin) {
			if(depset.find(*ddbegin) == depset.end()) {
				deplist.push_front(*ddbegin);
				depset.insert(*ddbegin);
			}
		}
	}

	void Component::getTransitiveDependencies(list<Component*>& dependencies) const {
		dependencies.clear();
		set<Component*> depset;
		getTransitiveDependenciesImpl(*this, dependencies, depset);
	}

}}

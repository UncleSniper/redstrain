#include "Component.hpp"
#include "FileArtifact.hpp"

using std::set;
using std::map;
using std::list;
using std::string;

namespace redengine {
namespace build {

	Component::Component(Type type, const string& name, const string& baseDirectory)
			: type(type), name(name), baseDirectory(baseDirectory) {}

	Component::Component(const Component& component)
			: ReferenceCounted(component), type(component.type), name(component.name),
			baseDirectory(component.baseDirectory), sourceDirectories(component.sourceDirectories),
			privateHeaders(component.privateHeaders), exposedHeaders(component.exposedHeaders) {
		ComponentIterator depbegin(dependencies.begin()), depend(dependencies.end());
		for(; depbegin != depend; ++depbegin)
			(*depbegin)->ref();
		map<const Language*, map<string, FileArtifact*> >::const_iterator h0begin(privateHeaders.begin()),
				h0end(privateHeaders.end());
		for(; h0begin != h0end; ++h0begin) {
			map<string, FileArtifact*>::const_iterator h1begin(h0begin->second.begin()),
					h1end(h0begin->second.end());
			for(; h1begin != h1end; ++h1begin)
				h1begin->second->ref();
		}
		h0begin = exposedHeaders.begin();
		h0end = exposedHeaders.end();
		for(; h0begin != h0end; ++h0begin) {
			map<string, FileArtifact*>::const_iterator h1begin(h0begin->second.begin()),
					h1end(h0begin->second.end());
			for(; h1begin != h1end; ++h1begin)
				h1begin->second->ref();
		}
	}

	Component::~Component() {
		ComponentIterator depbegin(dependencies.begin()), depend(dependencies.end());
		for(; depbegin != depend; ++depbegin)
			(*depbegin)->unref();
		map<const Language*, map<string, FileArtifact*> >::const_iterator h0begin(privateHeaders.begin()),
				h0end(privateHeaders.end());
		for(; h0begin != h0end; ++h0begin) {
			map<string, FileArtifact*>::const_iterator h1begin(h0begin->second.begin()),
					h1end(h0begin->second.end());
			for(; h1begin != h1end; ++h1begin)
				h1begin->second->unref();
		}
		h0begin = exposedHeaders.begin();
		h0end = exposedHeaders.end();
		for(; h0begin != h0end; ++h0begin) {
			map<string, FileArtifact*>::const_iterator h1begin(h0begin->second.begin()),
					h1end(h0begin->second.end());
			for(; h1begin != h1end; ++h1begin)
				h1begin->second->unref();
		}
	}

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

	bool Component::addPrivateHeader(const Language& language, const string& path, FileArtifact& file) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = privateHeaders.find(&language);
		map<string, FileArtifact*>* h1;
		if(it0 == privateHeaders.end()) {
			privateHeaders[&language] = map<string, FileArtifact*>();
			h1 = &privateHeaders[&language];
		}
		else
			h1 = &it0->second;
		map<string, FileArtifact*>::iterator it1 = h1->find(path);
		if(it1 == h1->end()) {
			(*h1)[path] = &file;
			file.ref();
			return true;
		}
		else if(it1->second == &file)
			return false;
		else {
			FileArtifact* old = it1->second;
			it1->second = &file;
			file.ref();
			old->unref();
			return true;
		}
	}

	bool Component::removePrivateHeader(const Language& language, const string& path) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = privateHeaders.find(&language);
		if(it0 == privateHeaders.end())
			return false;
		map<string, FileArtifact*>::iterator it1 = it0->second.find(path);
		if(it1 == it0->second.end())
			return false;
		FileArtifact* old = it1->second;
		it0->second.erase(it1);
		if(it0->second.empty())
			privateHeaders.erase(it0);
		old->unref();
		return true;
	}

	void Component::clearPrivateHeaders(const Language& language) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = privateHeaders.find(&language);
		if(it0 == privateHeaders.end())
			return;
		map<string, FileArtifact*>::const_iterator h1begin(it0->second.begin()), h1end(it0->second.end());
		for(; h1begin != h1end; ++h1begin)
			h1begin->second->unref();
		privateHeaders.erase(it0);
	}

	void Component::clearPrivateHeaders() {
		map<const Language*, map<string, FileArtifact*> >::const_iterator eh0begin(privateHeaders.begin()),
				eh0end(privateHeaders.end());
		for(; eh0begin != eh0end; ++eh0begin) {
			map<string, FileArtifact*>::const_iterator h1begin(eh0begin->second.begin()),
					h1end(eh0begin->second.end());
			for(; h1begin != h1end; ++h1begin)
				h1begin->second->ref();
		}
		privateHeaders.clear();
	}

	FileArtifact* Component::getPrivateHeader(const Language& language, const string& path) const {
		map<const Language*, map<string, FileArtifact*> >::const_iterator it0 = privateHeaders.find(&language);
		if(it0 == privateHeaders.end())
			return NULL;
		map<string, FileArtifact*>::const_iterator it1 = it0->second.find(path);
		return it1 == it0->second.end() ? NULL : it1->second;
	}

	bool Component::addExposedHeader(const Language& language, const string& path, FileArtifact& file) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = exposedHeaders.find(&language);
		map<string, FileArtifact*>* h1;
		if(it0 == exposedHeaders.end()) {
			exposedHeaders[&language] = map<string, FileArtifact*>();
			h1 = &exposedHeaders[&language];
		}
		else
			h1 = &it0->second;
		map<string, FileArtifact*>::iterator it1 = h1->find(path);
		if(it1 == h1->end()) {
			(*h1)[path] = &file;
			file.ref();
			return true;
		}
		else if(it1->second == &file)
			return false;
		else {
			FileArtifact* old = it1->second;
			it1->second = &file;
			file.ref();
			old->unref();
			return true;
		}
	}

	bool Component::removeExposedHeader(const Language& language, const string& path) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = exposedHeaders.find(&language);
		if(it0 == exposedHeaders.end())
			return false;
		map<string, FileArtifact*>::iterator it1 = it0->second.find(path);
		if(it1 == it0->second.end())
			return false;
		FileArtifact* old = it1->second;
		it0->second.erase(it1);
		if(it0->second.empty())
			exposedHeaders.erase(it0);
		old->unref();
		return true;
	}

	void Component::clearExposedHeaders(const Language& language) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = exposedHeaders.find(&language);
		if(it0 == exposedHeaders.end())
			return;
		map<string, FileArtifact*>::const_iterator h1begin(it0->second.begin()), h1end(it0->second.end());
		for(; h1begin != h1end; ++h1begin)
			h1begin->second->unref();
		exposedHeaders.erase(it0);
	}

	void Component::clearExposedHeaders() {
		map<const Language*, map<string, FileArtifact*> >::const_iterator eh0begin(exposedHeaders.begin()),
				eh0end(exposedHeaders.end());
		for(; eh0begin != eh0end; ++eh0begin) {
			map<string, FileArtifact*>::const_iterator h1begin(eh0begin->second.begin()),
					h1end(eh0begin->second.end());
			for(; h1begin != h1end; ++h1begin)
				h1begin->second->ref();
		}
		exposedHeaders.clear();
	}

	FileArtifact* Component::getExposedHeader(const Language& language, const string& path) const {
		map<const Language*, map<string, FileArtifact*> >::const_iterator it0 = exposedHeaders.find(&language);
		if(it0 == exposedHeaders.end())
			return NULL;
		map<string, FileArtifact*>::const_iterator it1 = it0->second.find(path);
		return it1 == it0->second.end() ? NULL : it1->second;
	}

}}

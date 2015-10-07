#include "Component.hpp"
#include "FileArtifact.hpp"
#include "ComponentTypeStringifier.hpp"

using std::set;
using std::map;
using std::list;
using std::string;

namespace redengine {
namespace build {

	// ======== FlavoredArtifact ========

	Component::FlavoredArtifact::FlavoredArtifact(Artifact& artifact, const Flavor& flavor, const Language& language)
			: artifact(&artifact), flavor(flavor), language(&language) {
		artifact.ref();
	}

	Component::FlavoredArtifact::FlavoredArtifact(const FlavoredArtifact& other)
			: artifact(other.artifact), flavor(other.flavor), language(other.language) {
		artifact->ref();
	}

	Component::FlavoredArtifact::~FlavoredArtifact() {
		artifact->unref();
	}

	Component::FlavoredArtifact& Component::FlavoredArtifact::operator=(const FlavoredArtifact& other) {
		other.artifact->ref();
		artifact->unref();
		artifact = other.artifact;
		flavor = other.flavor;
		language = other.language;
		return *this;
	}

	bool Component::FlavoredArtifact::operator==(const FlavoredArtifact& other) const {
		return artifact == other.artifact && flavor == other.flavor && language == other.language;
	}

	bool Component::FlavoredArtifact::operator!=(const FlavoredArtifact& other) const {
		return artifact != other.artifact || flavor != other.flavor || language != other.language;
	}

	bool Component::FlavoredArtifact::operator<(const FlavoredArtifact& other) const {
		if(artifact < other.artifact)
			return true;
		if(artifact > other.artifact)
			return false;
		if(flavor < other.flavor)
			return true;
		if(flavor > other.flavor)
			return false;
		return language < other.language;
	}

	bool Component::FlavoredArtifact::operator>(const FlavoredArtifact& other) const {
		if(artifact > other.artifact)
			return true;
		if(artifact < other.artifact)
			return false;
		if(flavor > other.flavor)
			return true;
		if(flavor < other.flavor)
			return false;
		return language > other.language;
	}

	// ======== Component ========

	Component::Component(Type type, const string& name, const string& baseDirectory)
			: type(type), name(name), baseDirectory(baseDirectory) {}

	Component::Component(const Component& component)
			: ReferenceCounted(component), type(component.type), name(component.name),
			baseDirectory(component.baseDirectory), sourceDirectories(component.sourceDirectories),
			privateHeaders(component.privateHeaders), exposedHeaders(component.exposedHeaders),
			finalArtifacts(component.finalArtifacts), unexposedHeaders(component.unexposedHeaders),
			reverseUnexposedHeaders(component.reverseUnexposedHeaders),
			exposeDirectories(component.exposeDirectories) {
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
		ConstUnexposedHeaderIterator uebegin(unexposedHeaders.begin()), ueend(unexposedHeaders.end());
		for(; uebegin != ueend; ++uebegin)
			uebegin->second->ref();
		uebegin = reverseUnexposedHeaders.begin();
		ueend = reverseUnexposedHeaders.end();
		for(; uebegin != ueend; ++uebegin)
			uebegin->second->ref();
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
		ConstUnexposedHeaderIterator uebegin(unexposedHeaders.begin()), ueend(unexposedHeaders.end());
		for(; uebegin != ueend; ++uebegin)
			uebegin->second->unref();
		uebegin = reverseUnexposedHeaders.begin();
		ueend = reverseUnexposedHeaders.end();
		for(; uebegin != ueend; ++uebegin)
			uebegin->second->unref();
	}

	void Component::setName(const string& name) {
		this->name = name;
	}

	string Component::getGoalName() const {
		return (type == EXECUTABLE ? "exe:" : "lib:") + name;
	}

	bool Component::addSourceDirectory(const string& directory) {
		return sourceDirectories.append(directory);
	}

	bool Component::removeSourceDirectory(const string& directory) {
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

	bool Component::addFinalArtifact(Artifact& artifact, const Flavor& flavor, const Language& language) {
		return finalArtifacts.append(FlavoredArtifact(artifact, flavor, language));
	}

	bool Component::removeFinalArtifact(Artifact& artifact, const Flavor& flavor, const Language& language) {
		return finalArtifacts.erase(FlavoredArtifact(artifact, flavor, language));
	}

	void Component::clearFinalArtifacts() {
		finalArtifacts.clear();
	}

	void Component::getFinalArtifacts(FlavoredArtifactIterator& begin, FlavoredArtifactIterator& end) const {
		begin = finalArtifacts.begin();
		end = finalArtifacts.end();
	}

	bool Component::addUnexposedHeader(const Artifact& exposed, FileArtifact& unexposed) {
		UnexposedHeaderIterator it = unexposedHeaders.find(&exposed);
		if(it == unexposedHeaders.end()) {
			unexposedHeaders[&exposed] = &unexposed;
			unexposed.ref();
			return true;
		}
		if(it->second == &unexposed)
			return false;
		unexposed.ref();
		it->second->unref();
		it->second = &unexposed;
		return true;
	}

	bool Component::removeUnexposedHeader(const Artifact& exposed) {
		UnexposedHeaderIterator it = unexposedHeaders.find(&exposed);
		if(it == unexposedHeaders.end())
			return false;
		it->second->unref();
		unexposedHeaders.erase(it);
		return true;
	}

	void Component::clearUnexposedHeaders() {
		ConstUnexposedHeaderIterator uebegin(unexposedHeaders.begin()), ueend(unexposedHeaders.end());
		for(; uebegin != ueend; ++uebegin)
			uebegin->second->unref();
		unexposedHeaders.clear();
	}

	FileArtifact* Component::getUnexposedHeader(const Artifact& exposed) const {
		ConstUnexposedHeaderIterator it = unexposedHeaders.find(&exposed);
		return it == unexposedHeaders.end() ? NULL : it->second;
	}

	bool Component::addReverseUnexposedHeader(const Artifact& unexposed, FileArtifact& exposed) {
		UnexposedHeaderIterator it = reverseUnexposedHeaders.find(&unexposed);
		if(it == reverseUnexposedHeaders.end()) {
			reverseUnexposedHeaders[&unexposed] = &exposed;
			exposed.ref();
			return true;
		}
		if(it->second == &exposed)
			return false;
		exposed.ref();
		it->second->unref();
		it->second = &exposed;
		return true;
	}

	bool Component::removeReverseUnexposedHeader(const Artifact& unexposed) {
		UnexposedHeaderIterator it = reverseUnexposedHeaders.find(&unexposed);
		if(it == reverseUnexposedHeaders.end())
			return false;
		it->second->unref();
		reverseUnexposedHeaders.erase(it);
		return true;
	}

	void Component::clearReverseUnexposedHeaders() {
		ConstUnexposedHeaderIterator uebegin(reverseUnexposedHeaders.begin()), ueend(reverseUnexposedHeaders.end());
		for(; uebegin != ueend; ++uebegin)
			uebegin->second->unref();
		reverseUnexposedHeaders.clear();
	}

	FileArtifact* Component::getReverseUnexposedHeader(const Artifact& unexposed) const {
		ConstUnexposedHeaderIterator it = reverseUnexposedHeaders.find(&unexposed);
		return it == reverseUnexposedHeaders.end() ? NULL : it->second;
	}

	bool Component::addHeaderExposeDirectory(const Language& language, const string& directory) {
		set<string>* s;
		map<const Language*, set<string> >::iterator it = exposeDirectories.find(&language);
		if(it == exposeDirectories.end()) {
			exposeDirectories[&language] = set<string>();
			s = &exposeDirectories[&language];
		}
		else
			s = &it->second;
		return s->insert(directory).second;
	}

	bool Component::removeHeaderExposeDirectory(const Language& language, const string& directory) {
		map<const Language*, set<string> >::iterator it = exposeDirectories.find(&language);
		if(it == exposeDirectories.end())
			return false;
		return !!it->second.erase(directory);
	}

	void Component::clearHeaderExposeDirectories(const Language& language) {
		exposeDirectories.erase(&language);
	}

	void Component::clearHeaderExposeDirectories() {
		exposeDirectories.clear();
	}

	static set<string> emptyStringSet;

	void Component::getHeaderExposeDirectories(const Language& language,
			ExposeDirectoryIterator& begin, ExposeDirectoryIterator& end) const {
		map<const Language*, set<string> >::const_iterator it = exposeDirectories.find(&language);
		if(it == exposeDirectories.end()) {
			begin = emptyStringSet.begin();
			end = emptyStringSet.end();
		}
		else {
			begin = it->second.begin();
			end = it->second.end();
		}
	}

	string::size_type Component::getMaximalComponentTypeWidth(ComponentTypeStringifier& typeStringifier) {
		string type(typeStringifier.stringifyComponentType(Component::LIBRARY));
		string::size_type max = type.length(), cur;
		type = typeStringifier.stringifyComponentType(Component::EXECUTABLE);
		cur = type.length();
		if(cur > max)
			max = cur;
		type = typeStringifier.stringifyComponentType(Component::DATA);
		cur = type.length();
		if(cur > max)
			max = cur;
		type = typeStringifier.stringifyComponentType(Component::BLOB);
		cur = type.length();
		if(cur > max)
			max = cur;
		return max;
	}

}}

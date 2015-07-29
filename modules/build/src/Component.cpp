#include <redstrain/util/Ref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "Flavor.hpp"
#include "Language.hpp"
#include "FileArtifact.hpp"
#include "BuildContext.hpp"
#include "RemoveAction.hpp"
#include "PresenceTrigger.hpp"

using std::map;
using std::set;
using std::list;
using std::string;
using redengine::util::Ref;
using redengine::util::Delete;
using redengine::util::Appender;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;

namespace redengine {
namespace build {

	static set<Artifact*> emptyArtifactSet;

	// ======== BuildDirectoryMapper ========

	Component::BuildDirectoryMapper::BuildDirectoryMapper() {}

	Component::BuildDirectoryMapper::BuildDirectoryMapper(const BuildDirectoryMapper&) {}

	Component::BuildDirectoryMapper::~BuildDirectoryMapper() {}

	// ======== PreciousArtifact ========

	Component::PreciousArtifact::PreciousArtifact(const Language& language, const Flavor& flavor,
			FileArtifact& artifact) : language(language), flavor(flavor), artifact(artifact) {
		artifact.ref();
	}

	Component::PreciousArtifact::PreciousArtifact(const PreciousArtifact& artifact)
			: ReferenceCounted(artifact), language(artifact.language), flavor(artifact.flavor),
			artifact(artifact.artifact) {
		this->artifact.ref();
	}

	Component::PreciousArtifact::~PreciousArtifact() {
		artifact.unref();
	}

	// ======== GenerationHolder ========

	Component::GenerationHolder::GenerationHolder() {}

	Component::GenerationHolder::GenerationHolder(const GenerationHolder& holder) : ReferenceCounted(holder) {}

	Component::GenerationHolder::~GenerationHolder() {}

	Component::PreciousArtifact* Component::GenerationHolder::getPreciousArtifact() {
		return NULL;
	}

	bool Component::GenerationHolder::evokesDependencySources() {
		return false;
	}

	void Component::GenerationHolder::addDependencySources(const Component&) {}

	// ======== DefaultGenerationHolder ========

	Component::DefaultGenerationHolder::DefaultGenerationHolder(
			GenerationTrigger* trigger) : trigger(trigger) {
		if(trigger)
			trigger->ref();
	}

	Component::DefaultGenerationHolder::DefaultGenerationHolder(const DefaultGenerationHolder& holder)
			: GenerationHolder(holder), trigger(holder.trigger) {
		if(trigger)
			trigger->ref();
	}

	Component::DefaultGenerationHolder::~DefaultGenerationHolder() {
		if(trigger)
			trigger->unref();
	}

	Trigger* Component::DefaultGenerationHolder::getTrigger() {
		return trigger;
	}

	void Component::DefaultGenerationHolder::addSource(FileArtifact* source) {
		if(source && trigger)
			trigger->addSource(source);
	}

	void Component::DefaultGenerationHolder::addTriggerSource(FileArtifact* source) {
		if(source && trigger)
			trigger->addOptionalSource(source);
	}

	void Component::DefaultGenerationHolder::getTargets(
			GenerationTrigger::ArtifactIterator& begin, GenerationTrigger::ArtifactIterator& end) {
		if(trigger)
			trigger->getTargets(begin, end);
		else
			begin = end = emptyArtifactSet.end();
	}

	// ======== ValveInjector ========

	Component::ValveInjector::ValveInjector() {}

	Component::ValveInjector::ValveInjector(const ValveInjector&) {}

	Component::ValveInjector::~ValveInjector() {}

	// ======== ComponentTypeStringifier ========

	Component::ComponentTypeStringifier::ComponentTypeStringifier() {}

	Component::ComponentTypeStringifier::ComponentTypeStringifier(const ComponentTypeStringifier&) {}

	Component::ComponentTypeStringifier::~ComponentTypeStringifier() {}

	// ======== BuildArtifactMapper ========

	Component::BuildArtifactMapper::BuildArtifactMapper() {}

	Component::BuildArtifactMapper::BuildArtifactMapper(const BuildArtifactMapper&) {}

	Component::BuildArtifactMapper::~BuildArtifactMapper() {}

	// ======== Component ========

	Component::Component(Type type, const string& name, const string& baseDirectory)
			: type(type), name(name), baseDirectory(baseDirectory) {}

	Component::Component(const Component& component)
			: ReferenceCounted(component), type(component.type), name(component.name),
			baseDirectory(component.baseDirectory), sourceDirectories(component.sourceDirectories),
			languages(component.languages), preciousArtifacts(component.preciousArtifacts),
			exposeDirectories(component.exposeDirectories), dependencies(component.dependencies),
			externalDependencies(component.externalDependencies), exposedHeaders(component.exposedHeaders),
			localHeaders(component.localHeaders), unexposedHeaders(component.unexposedHeaders) {
		PreciousArtifactIterator pabegin(preciousArtifacts.begin()), paend(preciousArtifacts.end());
		for(; pabegin != paend; ++pabegin)
			(*pabegin)->ref();
		ComponentIterator depbegin(dependencies.begin()), depend(dependencies.end());
		for(; depbegin != depend; ++depbegin)
			(*depbegin)->ref();
		map<const Language*, map<string, FileArtifact*> >::const_iterator eh0begin(exposedHeaders.begin()),
				eh0end(exposedHeaders.end());
		for(; eh0begin != eh0end; ++eh0begin) {
			map<string, FileArtifact*>::const_iterator eh1begin(eh0begin->second.begin()),
					eh1end(eh0begin->second.end());
			for(; eh1begin != eh1end; ++eh1begin)
				eh1begin->second->ref();
		}
		eh0begin = localHeaders.begin();
		eh0end = localHeaders.end();
		for(; eh0begin != eh0end; ++eh0begin) {
			map<string, FileArtifact*>::const_iterator eh1begin(eh0begin->second.begin()),
					eh1end(eh0begin->second.end());
			for(; eh1begin != eh1end; ++eh1begin)
				eh1begin->second->ref();
		}
		map<const Language*, map<FileArtifact*, FileArtifact*> >::const_iterator uh0begin(unexposedHeaders.begin()),
				uh0end(unexposedHeaders.end());
		for(; uh0begin != uh0end; ++uh0begin) {
			map<FileArtifact*, FileArtifact*>::const_iterator uh1begin(uh0begin->second.begin()),
					uh1end(uh0begin->second.end());
			for(; uh1begin != uh1end; ++uh1begin) {
				uh1begin->first->ref();
				uh1begin->second->ref();
			}
		}
	}

	Component::~Component() {
		PreciousArtifactIterator pabegin(preciousArtifacts.begin()), paend(preciousArtifacts.end());
		for(; pabegin != paend; ++pabegin)
			(*pabegin)->unref();
		ComponentIterator depbegin(dependencies.begin()), depend(dependencies.end());
		for(; depbegin != depend; ++depbegin)
			(*depbegin)->unref();
		map<const Language*, map<string, FileArtifact*> >::const_iterator eh0begin(exposedHeaders.begin()),
				eh0end(exposedHeaders.end());
		for(; eh0begin != eh0end; ++eh0begin) {
			map<string, FileArtifact*>::const_iterator eh1begin(eh0begin->second.begin()),
					eh1end(eh0begin->second.end());
			for(; eh1begin != eh1end; ++eh1begin)
				eh1begin->second->unref();
		}
		eh0begin = localHeaders.begin();
		eh0end = localHeaders.end();
		for(; eh0begin != eh0end; ++eh0begin) {
			map<string, FileArtifact*>::const_iterator eh1begin(eh0begin->second.begin()),
					eh1end(eh0begin->second.end());
			for(; eh1begin != eh1end; ++eh1begin)
				eh1begin->second->unref();
		}
		UnresolvedGenerationIterator ugbegin(unresolvedGenerations.begin()), ugend(unresolvedGenerations.end());
		for(; ugbegin != ugend; ++ugbegin)
			(*ugbegin)->unref();
		map<const Language*, map<FileArtifact*, FileArtifact*> >::const_iterator uh0begin(unexposedHeaders.begin()),
				uh0end(unexposedHeaders.end());
		for(; uh0begin != uh0end; ++uh0begin) {
			map<FileArtifact*, FileArtifact*>::const_iterator uh1begin(uh0begin->second.begin()),
					uh1end(uh0begin->second.end());
			for(; uh1begin != uh1end; ++uh1begin) {
				uh1begin->first->unref();
				uh1begin->second->unref();
			}
		}
	}

	void Component::setInternalBuildName(const string& name) {
		buildName = name;
	}

	void Component::addSourceDirectory(const string& directory) {
		if(!directory.empty())
			sourceDirectories.push_back(directory);
	}

	void Component::clearSourceDirectories() {
		sourceDirectories.clear();
	}

	void Component::getSourceDirectories(PathIterator& begin, PathIterator& end) const {
		begin = sourceDirectories.begin();
		end = sourceDirectories.end();
	}

	bool Component::addLanguage(Language* language) {
		if(!language)
			return false;
		return languages.insert(language).second;
	}

	void Component::clearLanguages() {
		languages.clear();
	}

	void Component::getLanguages(LanguageIterator& begin, LanguageIterator& end) const {
		begin = languages.begin();
		end = languages.end();
	}

	bool Component::addPreciousArtifact(PreciousArtifact* artifact) {
		if(!artifact)
			return false;
		if(!preciousArtifacts.insert(artifact).second)
			return false;
		artifact->ref();
		return true;
	}

	bool Component::removePreciousArtifact(PreciousArtifact* artifact) {
		if(!preciousArtifacts.erase(artifact))
			return false;
		artifact->unref();
		return true;
	}

	void Component::clearPreciousArtifacts() {
		PreciousArtifactIterator pabegin(preciousArtifacts.begin()), paend(preciousArtifacts.end());
		for(; pabegin != paend; ++pabegin)
			(*pabegin)->unref();
		preciousArtifacts.clear();
	}

	void Component::getPreciousArtifacts(PreciousArtifactIterator& begin, PreciousArtifactIterator& end) const {
		begin = preciousArtifacts.begin();
		end = preciousArtifacts.end();
	}

	bool Component::putHeaderExposeDirectory(const Language& language, const string& directory) {
		bool result = exposeDirectories.find(&language) == exposeDirectories.end();
		exposeDirectories[&language] = directory;
		return result;
	}

	bool Component::removeHeaderExposeDirectory(const Language& language) {
		return exposeDirectories.erase(&language);
	}

	string Component::getHeaderExposeDirectory(const Language& language) const {
		ExposeDirectories::const_iterator it = exposeDirectories.find(&language);
		return it == exposeDirectories.end() ? "" : it->second;
	}

	bool Component::addDependency(Component* component) {
		if(!component)
			return false;
		if(!dependencies.insert(component).second)
			return false;
		component->ref();
		return true;
	}

	bool Component::removeDependency(Component* component) {
		if(!dependencies.erase(component))
			return false;
		component->unref();
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

	bool Component::addExternalDependency(const Language& language, const string& library) {
		ExternalDependencies::iterator oit = externalDependencies.find(&language);
		set<string>* inner;
		bool insouter = oit == externalDependencies.end();
		if(insouter) {
			externalDependencies[&language] = set<string>();
			inner = &externalDependencies[&language];
		}
		else
			inner = &oit->second;
		if(!inner->insert(library).second)
			return insouter;
		return true;
	}

	bool Component::removeExternalDependency(const Language& language, const string& library) {
		ExternalDependencies::iterator oit = externalDependencies.find(&language);
		if(oit == externalDependencies.end())
			return false;
		return oit->second.erase(library);
	}

	void Component::clearExternalDependencies(const Language& language) {
		ExternalDependencies::iterator oit = externalDependencies.find(&language);
		if(oit != externalDependencies.end())
			oit->second.clear();
	}

	void Component::clearExternalDependencies() {
		externalDependencies.clear();
	}

	static set<string> emptyStringSet;

	void Component::getExternalDependencies(const Language& language,
			DependencyIterator& begin, DependencyIterator& end) const {
		ExternalDependencyIterator oit = externalDependencies.find(&language);
		if(oit == externalDependencies.end())
			begin = end = emptyStringSet.end();
		else {
			begin = oit->second.begin();
			end = oit->second.end();
		}
	}

	bool Component::addExposedHeader(const Language& language, const string& path,
			FileArtifact& file) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = exposedHeaders.find(&language);
		map<string, FileArtifact*>* eh1;
		if(it0 == exposedHeaders.end()) {
			exposedHeaders[&language] = map<string, FileArtifact*>();
			eh1 = &exposedHeaders[&language];
		}
		else
			eh1 = &it0->second;
		map<string, FileArtifact*>::iterator it1 = eh1->find(path);
		if(it1 == eh1->end()) {
			(*eh1)[path] = &file;
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

	void Component::clearExposedHeaders(const Language& language) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = exposedHeaders.find(&language);
		if(it0 == exposedHeaders.end())
			return;
		map<string, FileArtifact*>::const_iterator eh1begin(it0->second.begin()), eh1end(it0->second.end());
		for(; eh1begin != eh1end; ++eh1begin)
			eh1begin->second->unref();
		exposedHeaders.erase(it0);
	}

	void Component::clearExposedHeaders() {
		map<const Language*, map<string, FileArtifact*> >::const_iterator eh0begin(exposedHeaders.begin()),
				eh0end(exposedHeaders.end());
		for(; eh0begin != eh0end; ++eh0begin) {
			map<string, FileArtifact*>::const_iterator eh1begin(eh0begin->second.begin()),
					eh1end(eh0begin->second.end());
			for(; eh1begin != eh1end; ++eh1begin) {
				eh1begin->second->ref();
			}
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

	bool Component::addLocalHeader(const Language& language, const string& path,
			FileArtifact& file) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = localHeaders.find(&language);
		map<string, FileArtifact*>* eh1;
		if(it0 == localHeaders.end()) {
			localHeaders[&language] = map<string, FileArtifact*>();
			eh1 = &localHeaders[&language];
		}
		else
			eh1 = &it0->second;
		map<string, FileArtifact*>::iterator it1 = eh1->find(path);
		if(it1 == eh1->end()) {
			(*eh1)[path] = &file;
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

	void Component::clearLocalHeaders(const Language& language) {
		map<const Language*, map<string, FileArtifact*> >::iterator it0 = localHeaders.find(&language);
		if(it0 == localHeaders.end())
			return;
		map<string, FileArtifact*>::const_iterator eh1begin(it0->second.begin()), eh1end(it0->second.end());
		for(; eh1begin != eh1end; ++eh1begin)
			eh1begin->second->unref();
		localHeaders.erase(it0);
	}

	void Component::clearLocalHeaders() {
		map<const Language*, map<string, FileArtifact*> >::const_iterator eh0begin(localHeaders.begin()),
				eh0end(localHeaders.end());
		for(; eh0begin != eh0end; ++eh0begin) {
			map<string, FileArtifact*>::const_iterator eh1begin(eh0begin->second.begin()),
					eh1end(eh0begin->second.end());
			for(; eh1begin != eh1end; ++eh1begin) {
				eh1begin->second->ref();
			}
		}
		localHeaders.clear();
	}

	FileArtifact* Component::getLocalHeader(const Language& language, const string& path) const {
		map<const Language*, map<string, FileArtifact*> >::const_iterator it0 = localHeaders.find(&language);
		if(it0 == localHeaders.end())
			return NULL;
		map<string, FileArtifact*>::const_iterator it1 = it0->second.find(path);
		return it1 == it0->second.end() ? NULL : it1->second;
	}

	bool Component::addUnresolvedGeneration(GenerationHolder* holder) {
		if(!holder)
			return false;
		if(!unresolvedGenerations.insert(holder).second)
			return false;
		holder->ref();
		return true;
	}

	void Component::resolveUnresolvedGenerations() {
		while(!unresolvedGenerations.empty()) {
			UnresolvedGenerationIterator front = unresolvedGenerations.begin();
			GenerationHolder* ug = *front;
			ug->addDependencySources(*this);
			unresolvedGenerations.erase(front);
			ug->unref();
		}
	}

	void Component::addUnexposedHeader(const Language& language, FileArtifact* exposed, FileArtifact* unexposed) {
		if(!exposed || !unexposed)
			return;
		map<const Language*, map<FileArtifact*, FileArtifact*> >::iterator it0 = unexposedHeaders.find(&language);
		map<FileArtifact*, FileArtifact*>* uh1;
		if(it0 == unexposedHeaders.end()) {
			unexposedHeaders[&language] = map<FileArtifact*, FileArtifact*>();
			uh1 = &unexposedHeaders[&language];
		}
		else
			uh1 = &it0->second;
		map<FileArtifact*, FileArtifact*>::iterator it1 = uh1->find(exposed);
		if(it1 == uh1->end()) {
			(*uh1)[exposed] = unexposed;
			exposed->ref();
			unexposed->ref();
		}
		else if(it1->second != unexposed) {
			FileArtifact* old = it1->second;
			it1->second = unexposed;
			unexposed->ref();
			old->unref();
		}
	}

	void Component::clearUnexposedHeaders(const Language& language) {
		map<const Language*, map<FileArtifact*, FileArtifact*> >::iterator it0 = unexposedHeaders.find(&language);
		if(it0 == unexposedHeaders.end())
			return;
		map<FileArtifact*, FileArtifact*>::const_iterator uh1begin(it0->second.begin()), uh1end(it0->second.end());
		for(; uh1begin != uh1end; ++uh1begin) {
			uh1begin->first->unref();
			uh1begin->second->unref();
		}
		unexposedHeaders.erase(it0);
	}

	void Component::clearUnexposedHeaders() {
		map<const Language*, map<FileArtifact*, FileArtifact*> >::const_iterator uh0begin(unexposedHeaders.begin()),
				uh0end(unexposedHeaders.end());
		for(; uh0begin != uh0end; ++uh0begin) {
			map<FileArtifact*, FileArtifact*>::const_iterator uh1begin(uh0begin->second.begin()),
					uh1end(uh0begin->second.end());
			for(; uh1begin != uh1end; ++uh1begin) {
				uh1begin->first->unref();
				uh1begin->second->unref();
			}
		}
		unexposedHeaders.clear();
	}

	FileArtifact* Component::getUnexposedHeader(const Language& language, FileArtifact* exposed) const {
		map<const Language*, map<FileArtifact*, FileArtifact*> >::const_iterator it0
				= unexposedHeaders.find(&language);
		if(it0 == unexposedHeaders.end())
			return NULL;
		map<FileArtifact*, FileArtifact*>::const_iterator it1 = it0->second.find(exposed);
		return it1 == it0->second.end() ? NULL : it1->second;
	}

	struct PathPair {

		const string directory, basename;
		const Flavor flavor;

		PathPair(const string& directory, const string& basename, const Flavor& flavor)
				: directory(directory), basename(basename), flavor(flavor) {}

		PathPair(const PathPair& pair)
				: directory(pair.directory), basename(pair.basename), flavor(pair.flavor) {}

		bool operator<(const PathPair& pair) const {
			if(directory < pair.directory)
				return true;
			if(directory > pair.directory)
				return false;
			return basename < pair.basename;
		}

	};

	struct FlavorGraph {

		Component::GenerationHolder* singleTrigger;
		list<Component::GenerationHolder*> allTriggers;

		FlavorGraph() : singleTrigger(NULL) {}

		~FlavorGraph() {
			if(singleTrigger)
				singleTrigger->unref();
			list<Component::GenerationHolder*>::const_iterator begin(allTriggers.begin()), end(allTriggers.end());
			for(; begin != end; ++begin)
				(*begin)->unref();
		}

	};

	struct LanguageInfo {

		Language& language;
		list<PathPair> sources, headers;
		map<string, FlavorGraph*> graphs;

		LanguageInfo(Language& language) : language(language) {}

		LanguageInfo(const LanguageInfo& info)
				: language(info.language), sources(info.sources), headers(info.headers), graphs(info.graphs) {}

		~LanguageInfo() {
			map<string, FlavorGraph*>::const_iterator begin(graphs.begin()), end(graphs.end());
			for(; begin != end; ++begin)
				delete begin->second;
		}

		FlavorGraph& getOrMakeFlavorGraph(const Flavor& flavor) {
			const string& name = flavor.getName();
			map<string, FlavorGraph*>::const_iterator it = graphs.find(name);
			if(it != graphs.end())
				return *it->second;
			Delete<FlavorGraph> graph(new FlavorGraph);
			graphs[name] = *graph;
			return *graph.set();
		}

	};

	struct SetupTraverser : public Filesystem::TraversalSink {

		list<LanguageInfo>& languages;
		const string& sourceDirectory;
		bool useHeaders;

		SetupTraverser(list<LanguageInfo>& languages, const string& sourceDirectory, bool useHeaders)
				: languages(languages), sourceDirectory(sourceDirectory), useHeaders(useHeaders) {}

		virtual bool enterDirectory(const string&) {
			return true;
		}

		virtual void leaveDirectory(const string&, bool) {}

		virtual void visitFile(const string& path) {
			list<LanguageInfo>::iterator begin(languages.begin()), end(languages.end());
			for(; begin != end; ++begin) {
				switch(begin->language.classifyFile(path)) {
					case Language::AT_SOURCE:
						begin->sources.push_back(PathPair(sourceDirectory,
								Pathname::stripPrefix(path, sourceDirectory),
								begin->language.getShippedSourceFlavor()));
						break;
					case Language::AT_HEADER:
						if(useHeaders)
							begin->headers.push_back(PathPair(sourceDirectory,
									Pathname::stripPrefix(path, sourceDirectory),
									begin->language.getShippedHeaderFlavor()));
						break;
					default:
						break;
				}
			}
		}

	};

	struct FlavorAppender : public Appender<Flavor> {

		BuildContext& context;
		Component& component;
		LanguageInfo& language;
		list<LanguageInfo>& languages;
		Component::BuildDirectoryMapper& directoryMapper;
		Component::BuildArtifactMapper& artifactMapper;
		Component::ComponentTypeStringifier& typeStringifier;
		bool addedMoreSources;
		set<PathPair>& allBuildDirectories;

		FlavorAppender(BuildContext& context, Component& component, LanguageInfo& language,
				list<LanguageInfo>& languages, Component::BuildDirectoryMapper& directoryMapper,
				Component::BuildArtifactMapper& artifactMapper,
				Component::ComponentTypeStringifier& typeStringifier, set<PathPair>& allBuildDirectories)
				: context(context), component(component), language(language), languages(languages),
				directoryMapper(directoryMapper), artifactMapper(artifactMapper), typeStringifier(typeStringifier),
				addedMoreSources(false), allBuildDirectories(allBuildDirectories) {}

		virtual void append(const Flavor& flavor) {
			const string& cbase = component.getBaseDirectory();
			string ctailHead, ctailTail;
			directoryMapper.getBuildDirectory(component, language.language, flavor, ctailHead, ctailTail);
			string buildDirectory(Pathname::join(cbase, ctailHead));
			string fullBuildDirectory(Pathname::join(buildDirectory, ctailTail));
			bool cleanArtifact = buildDirectory == cbase;
			if(!cleanArtifact)
				allBuildDirectories.insert(PathPair(cbase, ctailHead, language.language.getCleanFlavor()));
			FlavorGraph& graph = language.getOrMakeFlavorGraph(flavor);
			list<PathPair>::const_iterator sbegin(language.sources.begin()), send(language.sources.end());
			bool isSingle = !language.language.isOneToOne(flavor);
			for(; sbegin != send; ++sbegin) {
				if(!language.language.acceptsSource(sbegin->flavor, flavor))
					continue;
				Component::GenerationHolder* newTrigger = NULL;
				if(isSingle) {
					if(graph.singleTrigger)
						graph.singleTrigger->addSource(context.internFileArtifact(sbegin->directory,
								sbegin->basename));
					else {
						newTrigger = language.language.getGenerationTrigger(context, sbegin->directory,
								sbegin->basename, sbegin->flavor, fullBuildDirectory, flavor, component,
								artifactMapper);
						graph.singleTrigger = newTrigger;
					}
				}
				else {
					Ref<Component::GenerationHolder> trigger(language.language.getGenerationTrigger(context,
							sbegin->directory, sbegin->basename, sbegin->flavor, fullBuildDirectory,
							flavor, component, artifactMapper));
					if(*trigger) {
						graph.allTriggers.push_back(*trigger);
						newTrigger = trigger.set();
					}
				}
				if(newTrigger) {
					GenerationTrigger::ArtifactIterator tbegin, tend;
					newTrigger->getTargets(tbegin, tend);
					for(; tbegin != tend; ++tbegin) {
						FileArtifact* file = dynamic_cast<FileArtifact*>(*tbegin);
						if(!file)
							continue;
						if(cleanArtifact)
							allBuildDirectories.insert(PathPair(file->getDirectory(), file->getBasename(),
									language.language.getCleanFlavor()));
						list<LanguageInfo>::iterator lbegin(languages.begin()), lend(languages.end());
						for(; lbegin != lend; ++lbegin) {
							switch(lbegin->language.classifyFile(file->getPathname())) {
								case Language::AT_SOURCE:
									lbegin->sources.push_back(PathPair(file->getDirectory(), file->getBasename(),
											language.language.getGeneratedSourceFlavor(sbegin->flavor, flavor,
											file->getBasename())));
									addedMoreSources = true;
									break;
								case Language::AT_HEADER:
									lbegin->headers.push_back(PathPair(file->getDirectory(), file->getBasename(),
											language.language.getGeneratedHeaderFlavor(sbegin->flavor, flavor,
											file->getBasename())));
									break;
								default:
									break;
							}
						}
					}
					string componentType(typeStringifier.stringifyComponentType(component.getType()));
					Trigger::ActionIterator abegin, aend;
					newTrigger->getTrigger()->getActions(abegin, aend);
					for(; abegin != aend; ++abegin) {
						(*abegin)->setComponentType(componentType);
						(*abegin)->setComponentName(component.getName());
						(*abegin)->setComponentBaseDirectory(component.getBaseDirectory());
					}
					Component::PreciousArtifact* preciousArtifact = newTrigger->getPreciousArtifact();
					if(preciousArtifact)
						component.addPreciousArtifact(preciousArtifact);
					if(newTrigger->evokesDependencySources())
						component.addUnresolvedGeneration(newTrigger);
				}
			}
		}

	};

	void Component::setupRules(BuildDirectoryMapper& directoryMapper, BuildArtifactMapper& artifactMapper,
			ComponentTypeStringifier& typeStringifier, BuildContext& context, ValveInjector* injector) {
		list<LanguageInfo> langinfo;
		LanguageIterator lbegin(languages.begin()), lend(languages.end());
		for(; lbegin != lend; ++lbegin)
			langinfo.push_back(LanguageInfo(**lbegin));
		// discover shipped sources
		PathIterator sdbegin(sourceDirectories.begin()), sdend(sourceDirectories.end());
		for(; sdbegin != sdend; ++sdbegin) {
			string srcdir(Pathname::join(baseDirectory, *sdbegin));
			SetupTraverser handler(langinfo, srcdir, type != EXECUTABLE);
			if(Filesystem::access(srcdir, Filesystem::FILE_EXISTS))
				Filesystem::traverse(srcdir, handler);
		}
		// process sources
		set<PathPair> allBuildDirectories;
		bool onceMoreWithFeeling;
		do {
			onceMoreWithFeeling = false;
			list<LanguageInfo>::iterator libegin(langinfo.begin()), liend(langinfo.end());
			for(; libegin != liend; ++libegin) {
				if(!libegin->sources.empty()) {
					FlavorAppender fhandler(context, *this, *libegin, langinfo, directoryMapper, artifactMapper,
							typeStringifier, allBuildDirectories);
					libegin->language.getSupportedFlavors(type, fhandler);
					libegin->sources.clear();
					if(fhandler.addedMoreSources)
						onceMoreWithFeeling = true;
				}
			}
		} while(onceMoreWithFeeling);
		// process headers, including generated ones
		string componentType(typeStringifier.stringifyComponentType(type));
		list<LanguageInfo>::iterator libegin(langinfo.begin()), liend(langinfo.end());
		for(; libegin != liend; ++libegin) {
			if(!libegin->headers.empty()) {
				Flavor heflavor = libegin->language.getHeaderExposeTransformFlavor();
				FlavorGraph& graph = libegin->getOrMakeFlavorGraph(heflavor);
				string edtailHead, edtailTail;
				directoryMapper.getHeaderExposeDirectory(*this, libegin->language, edtailHead, edtailTail);
				string exposeDirectory(Pathname::join(baseDirectory, edtailHead));
				string fullExposeDirectory(Pathname::join(exposeDirectory, edtailTail));
				putHeaderExposeDirectory(libegin->language, exposeDirectory);
				bool cleanArtifact = exposeDirectory == baseDirectory;
				if(!cleanArtifact)
					allBuildDirectories.insert(PathPair(baseDirectory, edtailHead,
							libegin->language.getCleanFlavor()));
				list<PathPair>::const_iterator hbegin(libegin->headers.begin()), hend(libegin->headers.end());
				for(; hbegin != hend; ++hbegin) {
					addLocalHeader(libegin->language, hbegin->basename,
							*context.internFileArtifact(hbegin->directory, hbegin->basename));
					GenerationHolder* newTrigger = NULL;
					Ref<GenerationHolder> trigger(libegin->language.getHeaderExposeTrigger(context,
							hbegin->directory, hbegin->basename, hbegin->flavor, fullExposeDirectory,
							heflavor, *this));
					if(*trigger) {
						graph.allTriggers.push_back(*trigger);
						newTrigger = trigger.set();
					}
					if(newTrigger) {
						FileArtifact* hesource = NULL;
						GenerationTrigger::ArtifactIterator tbegin, tend;
						newTrigger->getTargets(tbegin, tend);
						for(; tbegin != tend; ++tbegin) {
							FileArtifact* file = dynamic_cast<FileArtifact*>(*tbegin);
							if(file) {
								if(cleanArtifact)
									allBuildDirectories.insert(PathPair(file->getDirectory(), file->getBasename(),
											libegin->language.getCleanFlavor()));
								addExposedHeader(libegin->language,
										Pathname::stripPrefix(file->getPathname(), exposeDirectory), *file);
								if(!hesource)
									hesource = context.internFileArtifact(hbegin->directory, hbegin->basename);
								addUnexposedHeader(libegin->language, file, hesource);
							}
						}
						Trigger::ActionIterator abegin, aend;
						newTrigger->getTrigger()->getActions(abegin, aend);
						for(; abegin != aend; ++abegin) {
							(*abegin)->setComponentType(componentType);
							(*abegin)->setComponentName(name);
							(*abegin)->setComponentBaseDirectory(baseDirectory);
						}
						if(newTrigger->evokesDependencySources())
							addUnresolvedGeneration(newTrigger);
					}
				}
			}
		}
		// inject valves
		if(injector) {
			libegin = langinfo.begin();
			liend = langinfo.end();
			for(; libegin != liend; ++libegin) {
				map<string, FlavorGraph*>::const_iterator fgbegin(libegin->graphs.begin()),
						fgend(libegin->graphs.end());
				for(; fgbegin != fgend; ++fgbegin) {
					if(fgbegin->second->singleTrigger)
						injector->injectIntoTrigger(*fgbegin->second->singleTrigger->getTrigger(),
								*this, &libegin->language, Flavor(fgbegin->first), context);
					list<GenerationHolder*>::const_iterator ghbegin(fgbegin->second->allTriggers.begin()),
							ghend(fgbegin->second->allTriggers.end());
					for(; ghbegin != ghend; ++ghbegin)
						injector->injectIntoTrigger(*(*ghbegin)->getTrigger(), *this, &libegin->language,
								Flavor(fgbegin->first), context);
				}
			}
		}
		// commit to context
		while(!langinfo.empty()) {
			LanguageInfo& info = langinfo.front();
			while(!info.graphs.empty()) {
				map<string, FlavorGraph*>::iterator fgit(info.graphs.begin());
				FlavorGraph& graph = *fgit->second;
				if(graph.singleTrigger) {
					context.addTrigger(graph.singleTrigger->getTrigger());
					graph.singleTrigger->unref();
					graph.singleTrigger = NULL;
				}
				while(!graph.allTriggers.empty()) {
					GenerationHolder* trigger = graph.allTriggers.front();
					context.addTrigger(trigger->getTrigger());
					trigger->unref();
					graph.allTriggers.pop_front();
				}
				delete fgit->second;
				info.graphs.erase(fgit);
			}
			langinfo.pop_front();
		}
		// add clean actions
		set<PathPair>::const_iterator bdbegin(allBuildDirectories.begin()), bdend(allBuildDirectories.end());
		for(; bdbegin != bdend; ++bdbegin) {
			Delete<PresenceTrigger> trigger(new PresenceTrigger(PresenceTrigger::ANY));
			FileArtifact* file = context.internFileArtifact(bdbegin->directory, bdbegin->basename);
			trigger->addArtifact(file);
			Delete<RemoveAction> remove(new RemoveAction);
			remove->addArtifact(file);
			remove->setComponentType(typeStringifier.stringifyComponentType(type));
			remove->setComponentName(name);
			trigger->addAction(*remove);
			remove.set();
			if(injector)
				injector->injectIntoTrigger(**trigger, *this, NULL, bdbegin->flavor, context);
			context.addTrigger(*trigger);
			trigger.set();
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

#include <map>
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

	// ======== BuildDirectoryMapper ========

	Component::BuildDirectoryMapper::BuildDirectoryMapper() {}

	Component::BuildDirectoryMapper::BuildDirectoryMapper(const BuildDirectoryMapper&) {}

	Component::BuildDirectoryMapper::~BuildDirectoryMapper() {}

	// ======== GenerationHolder ========

	Component::GenerationHolder::GenerationHolder() {}

	Component::GenerationHolder::GenerationHolder(const GenerationHolder&) {}

	Component::GenerationHolder::~GenerationHolder() {}

	// ======== ValveInjector ========

	Component::ValveInjector::ValveInjector() {}

	Component::ValveInjector::ValveInjector(const ValveInjector&) {}

	Component::ValveInjector::~ValveInjector() {}

	// ======== Component ========

	Component::Component(Type type, const string& name, const string& baseDirectory)
			: type(type), name(name), baseDirectory(baseDirectory) {}

	Component::Component(const Component& component)
			: ReferenceCounted(component), type(component.type), name(component.name),
			baseDirectory(component.baseDirectory), sourceDirectories(component.sourceDirectories),
			languages(component.languages), preciousArtifacts(component.preciousArtifacts),
			exposeDirectories(component.exposeDirectories), dependencies(component.dependencies),
			externalDependencies(component.externalDependencies) {
		FileArtifactIterator pabegin(preciousArtifacts.begin()), paend(preciousArtifacts.end());
		for(; pabegin != paend; ++pabegin)
			(*pabegin)->ref();
		ComponentIterator depbegin(dependencies.begin()), depend(dependencies.end());
		for(; depbegin != depend; ++depbegin)
			(*depbegin)->ref();
	}

	Component::~Component() {
		FileArtifactIterator pabegin(preciousArtifacts.begin()), paend(preciousArtifacts.end());
		for(; pabegin != paend; ++pabegin)
			(*pabegin)->unref();
		ComponentIterator depbegin(dependencies.begin()), depend(dependencies.end());
		for(; depbegin != depend; ++depbegin)
			(*depbegin)->unref();
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

	bool Component::addPreciousArtifact(FileArtifact* file) {
		if(!file)
			return false;
		if(!preciousArtifacts.insert(file).second)
			return false;
		file->ref();
		return true;
	}

	bool Component::removePreciousArtifact(FileArtifact* file) {
		if(!preciousArtifacts.erase(file))
			return false;
		file->unref();
		return true;
	}

	void Component::clearPreciousArtifacts() {
		FileArtifactIterator pabegin(preciousArtifacts.begin()), paend(preciousArtifacts.end());
		for(; pabegin != paend; ++pabegin)
			(*pabegin)->unref();
		preciousArtifacts.clear();
	}

	void Component::getPreciousArtifacts(FileArtifactIterator& begin, FileArtifactIterator& end) const {
		begin = preciousArtifacts.begin();
		end = preciousArtifacts.end();
	}

	bool Component::putHeaderExposeDirectory(const Language& language, const string& directory) {
		bool result = exposeDirectories.find(language.getName()) == exposeDirectories.end();
		exposeDirectories[language.getName()] = directory;
		return result;
	}

	bool Component::removeHeaderExposeDirectory(const Language& language) {
		return exposeDirectories.erase(language.getName());
	}

	string Component::getHeaderExposeDirectory(const Language& language) const {
		ExposeDirectories::const_iterator it = exposeDirectories.find(language.getName());
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

	bool Component::addExternalDependency(const Language& language, const string& library) {
		const string& name = language.getName();
		ExternalDependencies::iterator oit = externalDependencies.find(name);
		set<string>* inner;
		bool insouter = oit == externalDependencies.end();
		if(insouter) {
			externalDependencies[name] = set<string>();
			inner = &externalDependencies[name];
		}
		else
			inner = &oit->second;
		if(!inner->insert(library).second)
			return insouter;
		return true;
	}

	bool Component::removeExternalDependency(const Language& language, const string& library) {
		const string& name = language.getName();
		ExternalDependencies::iterator oit = externalDependencies.find(name);
		if(oit == externalDependencies.end())
			return false;
		return oit->second.erase(library);
	}

	void Component::clearExternalDependencies(const Language& language) {
		ExternalDependencies::iterator oit = externalDependencies.find(language.getName());
		if(oit != externalDependencies.end())
			oit->second.clear();
	}

	void Component::clearExternalDependencies() {
		externalDependencies.clear();
	}

	static set<string> emptyStringSet;

	void Component::getExternalDependencies(const Language& language,
			DependencyIterator& begin, DependencyIterator& end) const {
		ExternalDependencyIterator oit = externalDependencies.find(language.getName());
		if(oit == externalDependencies.end())
			begin = end = emptyStringSet.end();
		else {
			begin = oit->second.begin();
			end = oit->second.end();
		}
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
				delete singleTrigger;
			list<Component::GenerationHolder*>::const_iterator begin(allTriggers.begin()), end(allTriggers.end());
			for(; begin != end; ++begin)
				delete *begin;
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
		bool addedMoreSources;
		set<PathPair>& allBuildDirectories;

		FlavorAppender(BuildContext& context, Component& component, LanguageInfo& language,
				list<LanguageInfo>& languages, Component::BuildDirectoryMapper& directoryMapper,
				set<PathPair>& allBuildDirectories)
				: context(context), component(component), language(language), languages(languages),
				directoryMapper(directoryMapper), addedMoreSources(false),
				allBuildDirectories(allBuildDirectories) {}

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
				Component::GenerationHolder* newTrigger = NULL;
				if(isSingle) {
					if(graph.singleTrigger)
						graph.singleTrigger->addSource(context.internFileArtifact(sbegin->directory,
								sbegin->basename));
					else {
						newTrigger = language.language.getGenerationTrigger(context, sbegin->directory,
								sbegin->basename, sbegin->flavor, fullBuildDirectory, flavor, component);
						graph.singleTrigger = newTrigger;
					}
				}
				else {
					Delete<Component::GenerationHolder> trigger(language.language.getGenerationTrigger(context,
							sbegin->directory, sbegin->basename, sbegin->flavor, fullBuildDirectory,
							flavor, component));
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
				}
			}
		}

	};

	void Component::setupRules(BuildDirectoryMapper& directoryMapper, BuildContext& context,
			ValveInjector* injector) {
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
					FlavorAppender fhandler(context, *this, *libegin, langinfo, directoryMapper,
							allBuildDirectories);
					libegin->language.getSupportedFlavors(type, fhandler);
					libegin->sources.clear();
					if(fhandler.addedMoreSources)
						onceMoreWithFeeling = true;
				}
			}
		} while(onceMoreWithFeeling);
		// process headers, including generated ones
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
					GenerationHolder* newTrigger = NULL;
					Delete<GenerationHolder> trigger(libegin->language.getHeaderExposeTrigger(context,
							hbegin->directory, hbegin->basename, hbegin->flavor, fullExposeDirectory, heflavor));
					if(*trigger) {
						graph.allTriggers.push_back(*trigger);
						newTrigger = trigger.set();
					}
					if(newTrigger && cleanArtifact) {
						GenerationTrigger::ArtifactIterator tbegin, tend;
						newTrigger->getTargets(tbegin, tend);
						for(; tbegin != tend; ++tbegin) {
							FileArtifact* file = dynamic_cast<FileArtifact*>(*tbegin);
							if(file)
								allBuildDirectories.insert(PathPair(file->getDirectory(), file->getBasename(),
										libegin->language.getCleanFlavor()));
						}
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
					delete graph.singleTrigger;
					graph.singleTrigger = NULL;
				}
				while(!graph.allTriggers.empty()) {
					GenerationHolder* trigger = graph.allTriggers.front();
					context.addTrigger(trigger->getTrigger());
					delete trigger;
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
			trigger->addAction(*remove);
			remove.set();
			if(injector)
				injector->injectIntoTrigger(**trigger, *this, NULL, bdbegin->flavor, context);
			context.addTrigger(*trigger);
			trigger.set();
		}
	}

}}

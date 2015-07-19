#include <map>
#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "Flavor.hpp"
#include "Language.hpp"
#include "FileArtifact.hpp"
#include "GenerationTrigger.hpp"

using std::map;
using std::list;
using std::string;
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

	// ======== Component ========

	Component::Component(Type type, const string& name, const string& baseDirectory)
			: type(type), name(name), baseDirectory(baseDirectory) {}

	Component::Component(const Component& component)
			: ReferenceCounted(component), type(component.type), name(component.name),
			baseDirectory(component.baseDirectory) {}

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

	struct PathPair {

		const string directory, basename;
		const Flavor flavor;

		PathPair(const string& directory, const string& basename, const Flavor& flavor)
				: directory(directory), basename(basename), flavor(flavor) {}

		PathPair(const PathPair& pair)
				: directory(pair.directory), basename(pair.basename), flavor(pair.flavor) {}

	};

	struct FlavorGraph {

		GenerationTrigger* singleTrigger;
		list<GenerationTrigger*> allTriggers;

		FlavorGraph() : singleTrigger(NULL) {}

		~FlavorGraph() {
			if(singleTrigger)
				delete singleTrigger;
			list<GenerationTrigger*>::const_iterator begin(allTriggers.begin()), end(allTriggers.end());
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

		SetupTraverser(list<LanguageInfo>& languages, const string& sourceDirectory)
				: languages(languages), sourceDirectory(sourceDirectory) {}

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
								Pathname::stripPrefix(path, sourceDirectory), Flavor("shipped")));
						break;
					case Language::AT_HEADER:
						begin->headers.push_back(PathPair(sourceDirectory,
								Pathname::stripPrefix(path, sourceDirectory), Flavor("shipped")));
						break;
					default:
						break;
				}
			}
		}

	};

	struct FlavorAppender : public Appender<Flavor> {

		const Component& component;
		LanguageInfo& language;
		list<LanguageInfo>& languages;
		Component::BuildDirectoryMapper& directoryMapper;
		bool addedMoreSources;

		FlavorAppender(const Component& component, LanguageInfo& language, list<LanguageInfo>& languages,
				Component::BuildDirectoryMapper& directoryMapper) : component(component), language(language),
				languages(languages), directoryMapper(directoryMapper), addedMoreSources(false) {}

		virtual void append(const Flavor& flavor) {
			string buildDirectory(Pathname::join(component.getBaseDirectory(),
					directoryMapper.getBuildDirectory(language.language, flavor)));
			FlavorGraph& graph = language.getOrMakeFlavorGraph(flavor);
			list<PathPair>::const_iterator sbegin(language.sources.begin()), send(language.sources.end());
			bool isSingle = language.language.isOneToOne(flavor);
			for(; sbegin != send; ++sbegin) {
				GenerationTrigger* newTrigger = NULL;
				if(isSingle) {
					if(graph.singleTrigger) {
						Delete<FileArtifact> file(new FileArtifact(sbegin->directory, sbegin->basename));
						graph.singleTrigger->addSource(*file);
						file.set();
					}
					else {
						newTrigger = language.language.getGenerationTrigger(sbegin->directory,
								sbegin->basename, sbegin->flavor, buildDirectory, flavor);
						graph.singleTrigger = newTrigger;
					}
				}
				else {
					Delete<GenerationTrigger> trigger(language.language.getGenerationTrigger(sbegin->directory,
							sbegin->basename, sbegin->flavor, buildDirectory, flavor));
					graph.allTriggers.push_back(*trigger);
					newTrigger = trigger.set();
				}
				if(newTrigger) {
					GenerationTrigger::ArtifactIterator tbegin, tend;
					newTrigger->getTargets(tbegin, tend);
					for(; tbegin != tend; ++tbegin) {
						FileArtifact* file = dynamic_cast<FileArtifact*>(*tbegin);
						if(!file)
							continue;
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

	void Component::setupRules(BuildDirectoryMapper& directoryMapper, BuildContext&) const {
		list<LanguageInfo> langinfo;
		LanguageIterator lbegin(languages.begin()), lend(languages.end());
		for(; lbegin != lend; ++lbegin)
			langinfo.push_back(LanguageInfo(**lbegin));
		// discover shipped sources
		PathIterator sdbegin(sourceDirectories.begin()), sdend(sourceDirectories.end());
		for(; sdbegin != sdend; ++sdbegin) {
			SetupTraverser handler(langinfo, *sdbegin);
			string srcdir(Pathname::join(baseDirectory, *sdbegin));
			if(Filesystem::access(srcdir, Filesystem::FILE_EXISTS))
				Filesystem::traverse(srcdir, handler);
		}
		// process sources
		bool onceMoreWithFeeling;
		do {
			onceMoreWithFeeling = false;
			list<LanguageInfo>::iterator libegin(langinfo.begin()), liend(langinfo.end());
			for(; libegin != liend; ++libegin) {
				if(!libegin->sources.empty()) {
					FlavorAppender fhandler(*this, *libegin, langinfo, directoryMapper);
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
				string exposeDirectory(Pathname::join(baseDirectory,
						directoryMapper.getHeaderExposeDirectory(libegin->language)));
				bool isSingle = libegin->language.isOneToOne(heflavor);
				list<PathPair>::const_iterator hbegin(libegin->headers.begin()), hend(libegin->headers.end());
				for(; hbegin != hend; ++hbegin) {
					if(isSingle) {
						if(graph.singleTrigger) {
							Delete<FileArtifact> file(new FileArtifact(hbegin->directory, hbegin->basename));
							graph.singleTrigger->addSource(*file);
							file.set();
						}
						else
							graph.singleTrigger = libegin->language.getHeaderExposeTrigger(hbegin->directory,
									hbegin->basename, hbegin->flavor, exposeDirectory, heflavor);
					}
					else {
						Delete<GenerationTrigger> trigger(libegin->language.getHeaderExposeTrigger(hbegin->directory,
								hbegin->basename, hbegin->flavor, exposeDirectory, heflavor));
						graph.allTriggers.push_back(*trigger);
						trigger.set();
					}
				}
			}
		}
		// TODO: commit to context
	}

}}

#include <redstrain/util/Unref.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "Language.hpp"
#include "Component.hpp"
#include "MultiGoal.hpp"
#include "RemoveGoal.hpp"
#include "BuildContext.hpp"
#include "ArtifactGoal.hpp"
#include "ComponentRuleBuilder.hpp"
#include "BuildDirectoryMapper.hpp"

using std::map;
using std::set;
using std::list;
using std::string;
using redengine::util::Unref;
using redengine::util::Appender;
using redengine::util::UniqueList;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;

namespace redengine {
namespace build {

	ComponentRuleBuilder::ComponentRuleBuilder(BuildDirectoryMapper& directoryMapper,
			BuildArtifactMapper& artifactMapper) : directoryMapper(directoryMapper),
			artifactMapper(artifactMapper) {}

	ComponentRuleBuilder::ComponentRuleBuilder(const ComponentRuleBuilder& builder)
			: RuleBuilder(builder), directoryMapper(builder.directoryMapper),
			artifactMapper(builder.artifactMapper) {}

	struct PendingHeaderScan {

		Language& language;
		Transform& transform;
		FileArtifact& source;

		PendingHeaderScan(Language& language, Transform& transform, FileArtifact& source)
				: language(language), transform(transform), source(source) {}

		PendingHeaderScan(const PendingHeaderScan& scan)
				: language(scan.language), transform(scan.transform), source(scan.source) {}

	};

	struct PerComponentSetupInfo {

		Component& component;
		BuildContext& context;
		ComponentRuleBuilder& builder;
		map<const Language*, ManyToOneTransform<FileArtifact>*> singleTransforms;
		list<PendingHeaderScan> pendingHeaderScans;
		UniqueList<FileArtifact*> buildDirectories;

		PerComponentSetupInfo(Component& component, BuildContext& context, ComponentRuleBuilder& builder)
				: component(component), context(context), builder(builder) {}

		~PerComponentSetupInfo() {
			map<const Language*, ManyToOneTransform<FileArtifact>*>::const_iterator
					stbegin(singleTransforms.begin()), stend(singleTransforms.end());
			for(; stbegin != stend; ++stbegin)
				stbegin->second->unref();
			list<PendingHeaderScan>::iterator phsbegin(pendingHeaderScans.begin()), phsend(pendingHeaderScans.end());
			for(; phsbegin != phsend; ++phsbegin) {
				phsbegin->transform.unref();
				phsbegin->source.unref();
			}
			UniqueList<FileArtifact*>::Iterator bdbegin(buildDirectories.begin()), bdend(buildDirectories.end());
			for(; bdbegin != bdend; ++bdbegin)
				(*bdbegin)->unref();
		}

	};

	struct SetupTraverser : Filesystem::TraversalSink {

		PerComponentSetupInfo& perComponent;
		const string& sourceDirectory;

		SetupTraverser(PerComponentSetupInfo& perComponent, const string& sourceDirectory)
				: perComponent(perComponent), sourceDirectory(sourceDirectory) {}

		void processSource(FileArtifact&, const Flavor&, Language&);
		void processHeader(FileArtifact&, const Flavor&, Language&);

		virtual bool enterDirectory(const string&) {
			return true;
		}

		virtual void leaveDirectory(const string&, bool) {}

		virtual void visitFile(const string& path) {
			Component::LanguageIterator lbegin, lend;
			perComponent.component.getLanguages(lbegin, lend);
			for(; lbegin != lend; ++lbegin) {
				switch((*lbegin)->classifyFile(path)) {
					case Language::AT_SOURCE:
						processSource(perComponent.context.internFileArtifact(path,
								Pathname::stripPrefix(path, sourceDirectory)),
								(*lbegin)->getShippedSourceFlavor(), **lbegin);
						break;
					case Language::AT_HEADER:
						processHeader(perComponent.context.internFileArtifact(path,
								Pathname::stripPrefix(path, sourceDirectory)),
								(*lbegin)->getShippedHeaderFlavor(), **lbegin);
						break;
					default:
						break;
				}
			}
		}

	};

	struct FlavorAppender : Appender<Flavor> {

		PerComponentSetupInfo& perComponent;
		const string& sourceDirectory;
		FileArtifact& sourceArtifact;
		const Flavor& sourceFlavor;
		Language& language;

		FlavorAppender(PerComponentSetupInfo& perComponent, const string& sourceDirectory,
				FileArtifact& sourceArtifact, const Flavor& sourceFlavor, Language& language)
				: perComponent(perComponent), sourceDirectory(sourceDirectory), sourceArtifact(sourceArtifact),
				sourceFlavor(sourceFlavor), language(language) {}

		virtual void append(const Flavor&);

	};

	struct ReferencedHeaderAppender : Appender<Language::ReferencedHeader> {

		const string& source;
		Component& component;
		bool allowPrivate;
		Language& language;
		Transform& transform;
		set<const FileArtifact*>& scanned;

		ReferencedHeaderAppender(const string& source, Component& component, bool allowPrivate,
				Language& language, Transform& transform, set<const FileArtifact*>& scanned)
				: source(source), component(component), allowPrivate(allowPrivate), language(language),
				transform(transform), scanned(scanned) {}

		virtual void append(const Language::ReferencedHeader&);

	};

	void SetupTraverser::processSource(FileArtifact& source, const Flavor& sourceFlavor, Language& language) {
		FlavorAppender sink(perComponent, sourceDirectory, source, sourceFlavor, language);
		language.getSupportedFlavors(perComponent.component.getType(), sink);
	}

	void SetupTraverser::processHeader(FileArtifact& header, const Flavor& headerFlavor, Language& language) {
		perComponent.component.addPrivateHeader(language,
				Pathname::tidy(Pathname::stripPrefix(header.getPath(), sourceDirectory)), header);
		if(perComponent.component.getType() != Component::EXECUTABLE) {
			const string& cbase = perComponent.component.getBaseDirectory();
			string ctailHead, ctailTail;
			perComponent.builder.getBuildDirectoryMapper().getHeaderExposeDirectory(perComponent.component,
					language, ctailHead, ctailTail);
			string exposeDirectory(Pathname::join(cbase, ctailHead));
			string fullExposeDirectory(Pathname::join(exposeDirectory, ctailTail));
			Flavor targetFlavor(Flavor::GENERIC);
			Unref<FileArtifact> target(language.getHeaderExposeTransform(perComponent.context, sourceDirectory,
					header, headerFlavor, fullExposeDirectory, perComponent.component,
					perComponent.builder.getBuildArtifactMapper(), targetFlavor));
			if(*target)
				perComponent.component.addExposedHeader(language,
						Pathname::tidy(Pathname::stripPrefix(target->getPath(), exposeDirectory)), **target);
		}
	}

	void FlavorAppender::append(const Flavor& transformFlavor) {
		if(!language.acceptsSource(sourceFlavor, transformFlavor))
			return;
		const string& cbase = perComponent.component.getBaseDirectory();
		string ctailHead, ctailTail;
		perComponent.builder.getBuildDirectoryMapper().getBuildDirectory(perComponent.component, language,
				transformFlavor, ctailHead, ctailTail);
		string buildDirectory(Pathname::tidy(Pathname::join(cbase, ctailHead)));
		string fullBuildDirectory(Pathname::join(buildDirectory, ctailTail));
		bool is121 = language.isOneToOne(transformFlavor);
		bool cleanArtifact = buildDirectory == Pathname::tidy(cbase);
		if(!is121) {
			map<const Language*, ManyToOneTransform<FileArtifact>*>::const_iterator it
					= perComponent.singleTransforms.find(&language);
			if(it != perComponent.singleTransforms.end()) {
				it->second->addPrerequisite(sourceArtifact);
				it->second->addSource(sourceArtifact);
				perComponent.pendingHeaderScans.push_back(PendingHeaderScan(language, *it->second, sourceArtifact));
				it->second->ref();
				sourceArtifact.ref();
				return;
			}
		}
		ManyToOneTransform<FileArtifact>* manyTransform = NULL;
		Flavor targetFlavor(Flavor::GENERIC);
		bool isFinal = false;
		Unref<FileArtifact> target(language.getSourceTransform(perComponent.context, sourceDirectory,
				sourceArtifact, sourceFlavor, fullBuildDirectory, transformFlavor, perComponent.component,
				perComponent.builder.getBuildArtifactMapper(), manyTransform, targetFlavor, isFinal));
		if(!*target)
			return;
		if(!is121 && manyTransform) {
			perComponent.singleTransforms[&language] = manyTransform;
			manyTransform->ref();
		}
		Transform* generatingTransform = target->getGeneratingTransform();
		if(generatingTransform) {
			perComponent.pendingHeaderScans.push_back(PendingHeaderScan(language,
					*generatingTransform, sourceArtifact));
			generatingTransform->ref();
			sourceArtifact.ref();
		}
		if(isFinal)
			perComponent.component.addFinalArtifact(**target, targetFlavor);
		if(cleanArtifact) {
			if(perComponent.buildDirectories.append(*target))
				target->ref();
		}
		else {
			FileArtifact& bdir = perComponent.context.internFileArtifact(buildDirectory,
					Pathname::stripPrefix(buildDirectory, Pathname::tidy(cbase)));
			if(perComponent.buildDirectories.append(&bdir))
				bdir.ref();
		}
		SetupTraverser followUp(perComponent, fullBuildDirectory);
		Component::LanguageIterator lbegin, lend;
		perComponent.component.getLanguages(lbegin, lend);
		for(; lbegin != lend; ++lbegin) {
			switch((*lbegin)->classifyFile(target->getPath())) {
				case Language::AT_SOURCE:
					followUp.processSource(**target, targetFlavor, **lbegin);
					break;
				case Language::AT_HEADER:
					followUp.processHeader(**target, targetFlavor, **lbegin);
					break;
				default:
					break;
			}
		}
	}

	void ComponentRuleBuilder::setupRules(const Project& project, Component& component, BuildContext& context) {
		if(context.hasComponentBeenBuilt(component))
			return;
		context.beginBuildingComponent(component);
		Component::ComponentIterator depbegin, depend;
		component.getDependencies(depbegin, depend);
		for(; depbegin != depend; ++depbegin)
			setupRules(project, **depbegin, context);
		setupRulesImpl(project, component, context);
		context.endBuildingComponent(component);
	}

	void ComponentRuleBuilder::setupRulesImpl(const Project&, Component& component, BuildContext& context) {
		PerComponentSetupInfo perComponent(component, context, *this);
		const string& baseDirectory = component.getBaseDirectory();
		// discover shipped sources
		Component::PathIterator sdbegin, sdend;
		component.getSourceDirectories(sdbegin, sdend);
		for(; sdbegin != sdend; ++sdbegin) {
			string srcdir(Pathname::join(baseDirectory, *sdbegin));
			SetupTraverser handler(perComponent, srcdir);
			if(Filesystem::access(srcdir, Filesystem::FILE_EXISTS))
				Filesystem::traverse(srcdir, handler);
		}
		// scan sources for extra dependencies via headers
		list<PendingHeaderScan>::iterator phsbegin(perComponent.pendingHeaderScans.begin()),
				phsend(perComponent.pendingHeaderScans.end());
		for(; phsbegin != phsend; ++phsbegin) {
			PendingHeaderScan& scan = *phsbegin;
			set<const FileArtifact*> scanned;
			ReferencedHeaderAppender sink(scan.source.getLabel(), component, true,
					scan.language, scan.transform, scanned);
			scan.language.getReferencedHeaders(scan.source.getPath(), sink);
		}
		// add goals
		Unref<MultiGoal> allGoal(new MultiGoal);
		map<Flavor, ArtifactGoal*> flavorGoals;
		Component::FlavoredArtifactIterator fabegin, faend;
		component.getFinalArtifacts(fabegin, faend);
		for(; fabegin != faend; ++fabegin) {
			map<Flavor, ArtifactGoal*>::const_iterator it = flavorGoals.find(fabegin->getFlavor());
			ArtifactGoal* fgoal;
			if(it == flavorGoals.end()) {
				Unref<ArtifactGoal> goal(new ArtifactGoal);
				string name(component.getGoalName() + ':' + fabegin->getFlavor().getName());
				if(context.addGoal(name, **goal)) {
					fgoal = *goal;
					flavorGoals[fabegin->getFlavor()] = fgoal;
					allGoal->addGoal(*fgoal);
				}
				else
					fgoal = dynamic_cast<ArtifactGoal*>(context.getGoal(name));
			}
			else
				fgoal = it->second;
			if(fgoal)
				fgoal->addArtifact(fabegin->getArtifact());
		}
		context.addGoal(component.getGoalName(), **allGoal);
		context.addGoal(component.getGoalName() + ":build", **allGoal);
		Unref<RemoveGoal> cleanGoal(new RemoveGoal);
		UniqueList<FileArtifact*>::Iterator bdbegin(perComponent.buildDirectories.begin()),
				bdend(perComponent.buildDirectories.end());
		for(; bdbegin != bdend; ++bdbegin)
			cleanGoal->addArtifact(**bdbegin);
		context.addGoal(component.getGoalName() + ":clean", **cleanGoal);
	}

	void ReferencedHeaderAppender::append(const Language::ReferencedHeader& header) {
		if(header.isLocal() && allowPrivate) {
			string lref(Pathname::tidy(Pathname::join(Pathname::dirname(source, Pathname::LOGICAL),
					header.getPath())));
			if(!Pathname::startsWith(lref, Pathname::PARENT_DIRECTORY)) {
				FileArtifact* pheader = component.getPrivateHeader(language, lref);
				if(pheader) {
					transform.addPrerequisite(*pheader);
					if(scanned.find(pheader) == scanned.end()) {
						scanned.insert(pheader);
						ReferencedHeaderAppender sink(pheader->getLabel(), component, true,
								language, transform, scanned);
						language.getReferencedHeaders(pheader->getPath(), sink);
					}
					return;
				}
			}
		}
		string gref(Pathname::tidy(header.getPath()));
		list<Component*> dependencies;
		component.getTransitiveDependencies(dependencies);
		list<Component*>::const_iterator depbegin(dependencies.begin()), depend(dependencies.end());
		for(; depbegin != depend; ++depbegin) {
			FileArtifact* eheader = (*depbegin)->getExposedHeader(language, gref);
			if(eheader) {
				transform.addPrerequisite(*eheader);
				FileArtifact* ueheader = (*depbegin)->getUnexposedHeader(*eheader);
				if(!ueheader)
					ueheader = eheader;
				if(scanned.find(ueheader) == scanned.end()) {
					scanned.insert(ueheader);
					ReferencedHeaderAppender sink(ueheader->getLabel(), **depbegin, false,
							language, transform, scanned);
					language.getReferencedHeaders(ueheader->getPath(), sink);
				}
				return;
			}
		}
	}

}}

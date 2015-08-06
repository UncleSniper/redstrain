#include <redstrain/util/Unref.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "Language.hpp"
#include "Component.hpp"
#include "BuildContext.hpp"
#include "ComponentRuleBuilder.hpp"
#include "BuildDirectoryMapper.hpp"

using std::map;
using std::string;
using redengine::util::Unref;
using redengine::util::Appender;
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

	struct PerComponentSetupInfo {

		Component& component;
		BuildContext& context;
		ComponentRuleBuilder& builder;
		map<const Language*, ManyToOneTransform<FileArtifact>*> singleTransforms;

		PerComponentSetupInfo(Component& component, BuildContext& context, ComponentRuleBuilder& builder)
				: component(component), context(context), builder(builder) {}

		~PerComponentSetupInfo() {
			map<const Language*, ManyToOneTransform<FileArtifact>*>::const_iterator
					stbegin(singleTransforms.begin()), stend(singleTransforms.end());
			for(; stbegin != stend; ++stbegin)
				stbegin->second->unref();
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

	void SetupTraverser::processSource(FileArtifact& source, const Flavor& sourceFlavor, Language& language) {
		FlavorAppender sink(perComponent, sourceDirectory, source, sourceFlavor, language);
		language.getSupportedFlavors(perComponent.component.getType(), sink);
	}

	void SetupTraverser::processHeader(FileArtifact& header, const Flavor& headerFlavor, Language& language) {
		perComponent.component.addPrivateHeader(language,
				Pathname::stripPrefix(header.getPath(), sourceDirectory), header);
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
			perComponent.component.addExposedHeader(language,
					Pathname::stripPrefix(target->getPath(), exposeDirectory), **target);
		}
	}

	void FlavorAppender::append(const Flavor& transformFlavor) {
		if(!language.acceptsSource(sourceFlavor, transformFlavor))
			return;
		const string& cbase = perComponent.component.getBaseDirectory();
		string ctailHead, ctailTail;
		perComponent.builder.getBuildDirectoryMapper().getBuildDirectory(perComponent.component, language,
				transformFlavor, ctailHead, ctailTail);
		string buildDirectory(Pathname::join(cbase, ctailHead));
		string fullBuildDirectory(Pathname::join(buildDirectory, ctailTail));
		bool is121 = language.isOneToOne(transformFlavor);
		if(!is121) {
			map<const Language*, ManyToOneTransform<FileArtifact>*>::const_iterator it
					= perComponent.singleTransforms.find(&language);
			if(it != perComponent.singleTransforms.end()) {
				it->second->addSource(sourceArtifact);
				return;
			}
		}
		ManyToOneTransform<FileArtifact>* manyTransform = NULL;
		Flavor targetFlavor(Flavor::GENERIC);
		Unref<FileArtifact> target(language.getSourceTransform(perComponent.context, sourceDirectory,
				sourceArtifact, sourceFlavor, fullBuildDirectory, transformFlavor, perComponent.component,
				perComponent.builder.getBuildArtifactMapper(), manyTransform, targetFlavor));
		if(!is121 && manyTransform) {
			perComponent.singleTransforms[&language] = manyTransform;
			manyTransform->ref();
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
	}

}}

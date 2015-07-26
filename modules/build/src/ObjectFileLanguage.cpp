#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>

#include "Linker.hpp"
#include "FileArtifact.hpp"
#include "BuildContext.hpp"
#include "ObjectFileLanguage.hpp"
#include "LinkGenerationAction.hpp"
#include "EmptyLinkerConfiguration.hpp"

using std::list;
using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::util::Appender;

namespace redengine {
namespace build {

	static list<Artifact*> emptyArtifactList;

	// ======== LinkGenerationHolder ========

	ObjectFileLanguage::LinkGenerationHolder::LinkGenerationHolder(GenerationTrigger* trigger,
			LinkGenerationAction* action, Component::PreciousArtifact* preciousArtifact)
			: trigger(trigger), action(action), preciousArtifact(preciousArtifact) {
		if(trigger)
			trigger->ref();
		if(preciousArtifact)
			preciousArtifact->ref();
	}

	ObjectFileLanguage::LinkGenerationHolder::LinkGenerationHolder(const LinkGenerationHolder& holder)
			: GenerationHolder(holder), trigger(holder.trigger), action(holder.action),
			preciousArtifact(holder.preciousArtifact) {
		if(trigger)
			trigger->ref();
		if(preciousArtifact)
			preciousArtifact->ref();
	}

	ObjectFileLanguage::LinkGenerationHolder::~LinkGenerationHolder() {
		if(trigger)
			trigger->unref();
		if(preciousArtifact)
			preciousArtifact->unref();
	}

	Trigger* ObjectFileLanguage::LinkGenerationHolder::getTrigger() {
		return trigger;
	}

	void ObjectFileLanguage::LinkGenerationHolder::addSource(FileArtifact* source) {
		if(!source)
			return;
		if(trigger)
			trigger->addSource(source);
		if(action)
			action->addSource(source);
	}

	void ObjectFileLanguage::LinkGenerationHolder::addTriggerSource(FileArtifact* source) {
		if(source && trigger)
			trigger->addSource(source);
	}

	void ObjectFileLanguage::LinkGenerationHolder::getTargets(GenerationTrigger::ArtifactIterator& begin,
			GenerationTrigger::ArtifactIterator& end) {
		if(trigger)
			trigger->getTargets(begin, end);
		else
			begin = end = emptyArtifactList.end();
	}

	Component::PreciousArtifact* ObjectFileLanguage::LinkGenerationHolder::getPreciousArtifact() {
		return preciousArtifact;
	}

	// ======== ObjectFileLanguage ========

	ObjectFileLanguage::ObjectFileLanguage(const string& name, Linker& linker)
			: Language(name), linker(linker) {}

	ObjectFileLanguage::ObjectFileLanguage(const ObjectFileLanguage& language)
			: Language(language), linker(language.linker) {}

	LinkerConfiguration& ObjectFileLanguage::getLinkerConfiguration(const Flavor&, const Component&) {
		return EmptyLinkerConfiguration::instance;
	}

	Language::ArtifactType ObjectFileLanguage::classifyFile(const string& path) {
		return linker.isObjectFile(path) ? AT_SOURCE : AT_NONE;
	}

	bool ObjectFileLanguage::acceptsSource(const Flavor& sourceFlavor, const Flavor& transformFlavor) {
		return sourceFlavor == transformFlavor;
	}

	static const Flavor *const FLAVORS[] = {
		&Flavor::STATIC,
		&Flavor::DYNAMIC,
		NULL
	};

	void ObjectFileLanguage::getSupportedFlavors(Component::Type, Appender<Flavor>& sink) {
		Language::sinkSupportedFlavors(FLAVORS, sink);
	}

	bool ObjectFileLanguage::isOneToOne(const Flavor&) {
		return false;
	}

	Component::GenerationHolder* ObjectFileLanguage::getGenerationTrigger(BuildContext& context,
			const string& sourceDirectory, const string& sourceBasename, const Flavor&,
			const string& targetDirectory, const Flavor& transformFlavor, Component& component,
			Component::BuildArtifactMapper& artifactMapper) {
		Linkage::LinkMode mode;
		if(transformFlavor == Flavor::STATIC)
			mode = component.getType() == Component::EXECUTABLE
					? Linkage::STATIC_EXECUTABLE : Linkage::STATIC_LIBRARY;
		else if(transformFlavor == Flavor::DYNAMIC)
			mode = component.getType() == Component::EXECUTABLE
					? Linkage::DYNAMIC_EXECUTABLE : Linkage::DYNAMIC_LIBRARY;
		else
			return NULL;
		string targetBasename(linker.decorateBinaryFileName(artifactMapper.getTargetFileName(component,
				*this, transformFlavor), mode));
		FileArtifact* srcfile = context.internFileArtifact(sourceDirectory, sourceBasename);
		FileArtifact* trgfile = context.internFileArtifact(targetDirectory, targetBasename);
		Delete<GenerationTrigger> trigger(new GenerationTrigger);
		trigger->addSource(srcfile);
		trigger->addTarget(trgfile);
		Unref<LinkGenerationAction> action(new LinkGenerationAction(trgfile, linker, mode,
				getLinkerConfiguration(transformFlavor, component)));
		action->addSource(srcfile);
		trigger->addAction(*action);
		Unref<Component::PreciousArtifact> preciousArtifact(new Component::PreciousArtifact(*this,
				transformFlavor, *trgfile));
		LinkGenerationHolder* holder = new LinkGenerationHolder(*trigger, *action, *preciousArtifact);
		trigger.set();
		return holder;
	}

	Flavor ObjectFileLanguage::getGeneratedSourceFlavor(const Flavor&, const Flavor& transformFlavor,
			const string&) {
		return transformFlavor;
	}

	Flavor ObjectFileLanguage::getGeneratedHeaderFlavor(const Flavor&, const Flavor& transformFlavor,
			const string&) {
		return transformFlavor;
	}

	Component::GenerationHolder* ObjectFileLanguage::getHeaderExposeTrigger(BuildContext&, const string&,
			const string&, const Flavor&, const string&, const Flavor&) {
		return NULL;
	}

}}

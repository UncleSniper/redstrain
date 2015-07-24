#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>

#include "Compiler.hpp"
#include "FileArtifact.hpp"
#include "FileCopyAction.hpp"
#include "CompiledLanguage.hpp"
#include "CompileGenerationAction.hpp"
#include "EmptyCompilerConfiguration.hpp"

using std::list;
using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::util::Appender;

namespace redengine {
namespace build {

	static list<Artifact*> emptyArtifactList;

	// ======== CompileGenerationHolder ========

	CompiledLanguage::CompileGenerationHolder::CompileGenerationHolder(GenerationTrigger* trigger,
			CompileGenerationAction* action) : trigger(trigger), action(action) {
		if(trigger)
			trigger->ref();
	}

	CompiledLanguage::CompileGenerationHolder::CompileGenerationHolder(const CompileGenerationHolder& holder)
			: GenerationHolder(holder), trigger(holder.trigger), action(holder.action) {
		if(trigger)
			trigger->ref();
	}

	CompiledLanguage::CompileGenerationHolder::~CompileGenerationHolder() {
		if(trigger)
			trigger->unref();
	}

	Trigger* CompiledLanguage::CompileGenerationHolder::getTrigger() {
		return trigger;
	}

	void CompiledLanguage::CompileGenerationHolder::addSource(FileArtifact* source) {
		if(!source)
			return;
		if(trigger)
			trigger->addSource(source);
		if(action)
			action->addSource(source);
	}

	void CompiledLanguage::CompileGenerationHolder::getTargets(GenerationTrigger::ArtifactIterator& begin,
			GenerationTrigger::ArtifactIterator& end) {
		if(trigger)
			trigger->getTargets(begin, end);
		else
			begin = end = emptyArtifactList.end();
	}

	// ======== ExposeGenerationHolder ========

	CompiledLanguage::ExposeGenerationHolder::ExposeGenerationHolder(GenerationTrigger* trigger)
			: trigger(trigger) {}

	CompiledLanguage::ExposeGenerationHolder::ExposeGenerationHolder(const ExposeGenerationHolder& holder)
			: GenerationHolder(holder), trigger(holder.trigger) {
		if(trigger)
			trigger->ref();
	}

	CompiledLanguage::ExposeGenerationHolder::~ExposeGenerationHolder() {
		if(trigger)
			trigger->unref();
	}

	Trigger* CompiledLanguage::ExposeGenerationHolder::getTrigger() {
		return trigger;
	}

	void CompiledLanguage::ExposeGenerationHolder::addSource(FileArtifact* source) {
		if(source && trigger)
			trigger->addSource(source);
	}

	void CompiledLanguage::ExposeGenerationHolder::getTargets(GenerationTrigger::ArtifactIterator& begin,
			GenerationTrigger::ArtifactIterator& end) {
		if(trigger)
			trigger->getTargets(begin, end);
		else
			begin = end = emptyArtifactList.end();
	}

	// ======== CompiledLanguage ========

	CompiledLanguage::CompiledLanguage(const string& name, Compiler& compiler)
			: Language(name), compiler(compiler) {}

	CompiledLanguage::CompiledLanguage(const CompiledLanguage& language)
			: Language(language), compiler(language.compiler) {}

	static const Flavor *const FLAVORS[] = {
		&Flavor::STATIC,
		&Flavor::DYNAMIC,
		NULL
	};

	void CompiledLanguage::getSupportedFlavors(Component::Type, Appender<Flavor>& sink) {
		Language::sinkSupportedFlavors(FLAVORS, sink);
	}

	bool CompiledLanguage::isOneToOne(const Flavor&) {
		return true;
	}

	Component::GenerationHolder* CompiledLanguage::getGenerationTrigger(const string& sourceDirectory,
			const string& sourceBasename, const Flavor&, const string& targetDirectory,
			const Flavor& transformFlavor, Component& component) {
		Compilation::CompileMode mode;
		if(transformFlavor == Flavor::STATIC)
			mode = component.getType() == Component::EXECUTABLE
					? Compilation::FOR_STATIC_EXECUTABLE : Compilation::FOR_STATIC_LIBRARY;
		else if(transformFlavor == Flavor::DYNAMIC)
			mode = component.getType() == Component::EXECUTABLE
					? Compilation::FOR_DYNAMIC_EXECUTABLE : Compilation::FOR_DYNAMIC_LIBRARY;
		else
			return NULL;
		string targetBasename(compiler.getObjectFileNameForSource(sourceBasename));
		Unref<FileArtifact> srcfile(new FileArtifact(sourceDirectory, sourceBasename));
		Unref<FileArtifact> trgfile(new FileArtifact(targetDirectory, targetBasename));
		Delete<GenerationTrigger> trigger(new GenerationTrigger);
		trigger->addSource(*srcfile);
		trigger->addTarget(*trgfile);
		Unref<CompileGenerationAction> action(new CompileGenerationAction(*trgfile, compiler, mode,
				getCompilerConfiguration(transformFlavor, component)));
		action->addSource(*srcfile);
		trigger->addAction(*action);
		CompileGenerationHolder* holder = new CompileGenerationHolder(*trigger, *action);
		trigger.set();
		return holder;
	}

	Flavor CompiledLanguage::getGeneratedSourceFlavor(const Flavor&, const Flavor& transformFlavor, const string&) {
		return transformFlavor;
	}

	Flavor CompiledLanguage::getGeneratedHeaderFlavor(const Flavor&, const Flavor& transformFlavor, const string&) {
		return transformFlavor;
	}

	Component::GenerationHolder* CompiledLanguage::getHeaderExposeTrigger(const string& sourceDirectory,
			const string& sourceBasename, const Flavor&, const string& targetDirectory, const Flavor&) {
		Unref<FileArtifact> srcfile(new FileArtifact(sourceDirectory, sourceBasename));
		Unref<FileArtifact> destfile(new FileArtifact(targetDirectory, sourceBasename));
		Delete<GenerationTrigger> trigger(new GenerationTrigger);
		trigger->addSource(*srcfile);
		trigger->addTarget(*destfile);
		Unref<FileCopyAction> action(new FileCopyAction(**srcfile, **destfile));
		trigger->addAction(*action);
		ExposeGenerationHolder* holder = new ExposeGenerationHolder(*trigger);
		trigger.set();
		return holder;
	}

	CompilerConfiguration& CompiledLanguage::getCompilerConfiguration(const Flavor&, const Component&) {
		return EmptyCompilerConfiguration::instance;
	}

}}

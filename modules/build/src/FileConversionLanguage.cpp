#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>

#include "FileArtifact.hpp"
#include "FileConversionLanguage.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::util::Appender;

namespace redengine {
namespace build {

	FileConversionLanguage::FileConversionLanguage(const string& name) : Language(name) {}

	FileConversionLanguage::FileConversionLanguage(const FileConversionLanguage& language) : Language(language) {}

	bool FileConversionLanguage::acceptsSource(const Flavor&, const Flavor&) {
		return true;
	}

	static const Flavor *const FLAVORS[] = {
		&Flavor::GENERIC,
		NULL
	};

	void FileConversionLanguage::getSupportedFlavors(Component::Type, Appender<Flavor>& sink) {
		Language::sinkSupportedFlavors(FLAVORS, sink);
	}

	bool FileConversionLanguage::isOneToOne(const Flavor&) {
		return true;
	}

	Component::GenerationHolder* FileConversionLanguage::getGenerationTrigger(BuildContext& context,
			const string& sourceDirectory, const string& sourceBasename, const Flavor& sourceFlavor,
			const string& targetDirectory, const Flavor& transformFlavor, Component& component,
			Component::BuildArtifactMapper&) {
		string targetBasename(getTargetBasename(sourceBasename, sourceFlavor, transformFlavor, component));
		FileArtifact* srcfile = context.internFileArtifact(sourceDirectory, sourceBasename);
		FileArtifact* trgfile = context.internFileArtifact(targetDirectory, targetBasename);
		Delete<GenerationTrigger> trigger(new GenerationTrigger);
		trigger->addSource(srcfile);
		trigger->addTarget(trgfile);
		Unref<GenerationAction<FileArtifact> > action(newGenerationAction(trgfile,
				sourceFlavor, transformFlavor, component, context));
		action->addSource(srcfile);
		trigger->addAction(*action);
		Component::DefaultGenerationHolder* holder = new Component::DefaultGenerationHolder(*trigger);
		trigger.set();
		return holder;
	}

	Flavor FileConversionLanguage::getGeneratedSourceFlavor(const Flavor&, const Flavor&, const string&) {
		return Flavor::GENERIC;
	}

	Flavor FileConversionLanguage::getGeneratedHeaderFlavor(const Flavor&, const Flavor&, const string&) {
		return Flavor::GENERIC;
	}

	Component::GenerationHolder* FileConversionLanguage::getHeaderExposeTrigger(BuildContext&, const string&,
			const string&, const Flavor&, const string&, const Flavor&, Component&) {
		return NULL;
	}

}}

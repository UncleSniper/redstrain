#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>

#include "FileArtifact.hpp"
#include "CodeTableDefinitionLanguage.hpp"
#include "CodeTableCompileGenerationAction.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::util::Appender;
using redengine::util::StringUtils;

namespace redengine {
namespace build {

	CodeTableDefinitionLanguage::CodeTableDefinitionLanguage() : Language("code table definition") {}

	CodeTableDefinitionLanguage::CodeTableDefinitionLanguage(const CodeTableDefinitionLanguage& language)
			: Language(language) {}

	static const char *const SOURCE_SUFFIXES[] = {
		".ctdef",
		NULL
	};

	Language::ArtifactType CodeTableDefinitionLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, NULL);
	}

	bool CodeTableDefinitionLanguage::acceptsSource(const Flavor&, const Flavor&) {
		return true;
	}

	static const Flavor *const FLAVORS[] = {
		&Flavor::GENERIC,
		NULL
	};

	void CodeTableDefinitionLanguage::getSupportedFlavors(Component::Type, Appender<Flavor>& sink) {
		Language::sinkSupportedFlavors(FLAVORS, sink);
	}

	bool CodeTableDefinitionLanguage::isOneToOne(const Flavor&) {
		return true;
	}

	Component::GenerationHolder* CodeTableDefinitionLanguage::getGenerationTrigger(BuildContext& context,
			const string& sourceDirectory, const string& sourceBasename, const Flavor&,
			const string& targetDirectory, const Flavor&, Component& component,
			Component::BuildArtifactMapper&) {
		string targetBasename(StringUtils::endsWith(sourceBasename, ".ctdef")
				? sourceBasename.substr(static_cast<string::size_type>(0u),
				sourceBasename.length() - static_cast<string::size_type>(6u))
				: sourceBasename);
		targetBasename += ".redct";
		FileArtifact* srcfile = context.internFileArtifact(sourceDirectory, sourceBasename);
		FileArtifact* trgfile = context.internFileArtifact(targetDirectory, targetBasename);
		Delete<GenerationTrigger> trigger(new GenerationTrigger);
		trigger->addSource(srcfile);
		trigger->addTarget(trgfile);
		Unref<CodeTableCompileGenerationAction> action(new CodeTableCompileGenerationAction(trgfile));
		action->addSource(srcfile);
		action->addIntermediateDirectories(component, context);
		trigger->addAction(*action);
		Component::DefaultGenerationHolder* holder = new Component::DefaultGenerationHolder(*trigger);
		trigger.set();
		return holder;
	}

	Flavor CodeTableDefinitionLanguage::getGeneratedSourceFlavor(const Flavor&, const Flavor&, const string&) {
		return Flavor::GENERIC;
	}

	Flavor CodeTableDefinitionLanguage::getGeneratedHeaderFlavor(const Flavor&, const Flavor&, const string&) {
		return Flavor::GENERIC;
	}

	Component::GenerationHolder* CodeTableDefinitionLanguage::getHeaderExposeTrigger(BuildContext&, const string&,
			const string&, const Flavor&, const string&, const Flavor&, Component&) {
		return NULL;
	}

}}

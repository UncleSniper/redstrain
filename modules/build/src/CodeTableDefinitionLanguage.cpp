#include "CodeTableDefinitionLanguage.hpp"

using std::string;
using redengine::util::Appender;

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

	Component::GenerationHolder* CodeTableDefinitionLanguage::getGenerationTrigger(BuildContext&, const string&,
			const string&, const Flavor&, const string&, const Flavor&, Component&,
			Component::BuildArtifactMapper&) {
		//TODO
		return NULL;
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

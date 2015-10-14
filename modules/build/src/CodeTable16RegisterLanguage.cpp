#include <redstrain/util/Unref.hpp>

#include "CodeTable16RegistrarGenerationTransform.hpp"

using std::string;
using redengine::util::Unref;

namespace redengine {
namespace build {

	// ======== RegistrarConfiguration ========

	CodeTable16RegisterLanguage::RegistrarConfiguration::RegistrarConfiguration() {}

	CodeTable16RegisterLanguage::RegistrarConfiguration::RegistrarConfiguration(const
			RegistrarConfiguration& configuration) : ReferenceCounted(configuration) {}

	CodeTable16RegisterLanguage::RegistrarConfiguration::~RegistrarConfiguration() {}

	// ======== CodeTable16RegisterLanguage ========

	CodeTable16RegisterLanguage::CodeTable16RegisterLanguage() : FileConversionLanguage("blob code table") {}

	CodeTable16RegisterLanguage::CodeTable16RegisterLanguage(const CodeTable16RegisterLanguage& language)
			: FileConversionLanguage(language) {}

	string CodeTable16RegisterLanguage::getTargetBasename(const string& sourceBasename,
			const Flavor&, const Flavor&, Component&) {
		return sourceBasename + ".cpp";
	}

	Transform* CodeTable16RegisterLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor& targetFlavor,
			const Flavor& transformFlavor, Component& component) {
		Unref<RegistrarConfiguration> configuration(getRegistrarConfiguration(sourceArtifact,
				sourceFlavor, targetArtifact, targetFlavor, transformFlavor, component));
		return new CodeTable16RegistrarGenerationTransform(sourceArtifact, *configuration);
	}

	CodeTable16RegisterLanguage::RegistrarConfiguration* CodeTable16RegisterLanguage::getRegistrarConfiguration(
			FileArtifact&, const Flavor&, FileArtifact&, const Flavor&, const Flavor&, Component&) {
		return NULL;
	}

	static const char *const SOURCE_SUFFIXES[] = {
		".ctrg2",
		NULL
	};

	Language::ArtifactType CodeTable16RegisterLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, NULL);
	}

}}

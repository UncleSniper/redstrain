#include <redstrain/util/Unref.hpp>

#include "BlobAliasLanguage.hpp"
#include "BlobAliasGenerationTransform.hpp"

using std::string;
using redengine::util::Unref;
using redengine::vfs::BlobLinkerDefinitionGenerator;

namespace redengine {
namespace build {

	// ======== AliasConfiguration ========

	BlobAliasLanguage::AliasConfiguration::AliasConfiguration() {}

	BlobAliasLanguage::AliasConfiguration::AliasConfiguration(const AliasConfiguration& configuration)
			: ReferenceCounted(configuration) {}

	BlobAliasLanguage::AliasConfiguration::~AliasConfiguration() {}

	// ======== GenericAliasConfiguration ========

	BlobAliasLanguage::GenericAliasConfiguration::GenericAliasConfiguration() {}

	BlobAliasLanguage::GenericAliasConfiguration::GenericAliasConfiguration(const string& pathPrefix,
			const string& fileSuffix) : pathPrefix(pathPrefix), fileSuffix(fileSuffix) {}

	BlobAliasLanguage::GenericAliasConfiguration::GenericAliasConfiguration(const
			GenericAliasConfiguration& configuration) : AliasConfiguration(configuration),
			pathPrefix(configuration.pathPrefix), fileSuffix(configuration.fileSuffix) {}

	BlobAliasLanguage::GenericAliasConfiguration::~GenericAliasConfiguration() {}

	void BlobAliasLanguage::GenericAliasConfiguration::setPathPrefix(const string& prefix) {
		pathPrefix = prefix;
	}

	void BlobAliasLanguage::GenericAliasConfiguration::setFileSuffix(const string& suffix) {
		fileSuffix = suffix;
	}

	void BlobAliasLanguage::GenericAliasConfiguration::applyConfiguration(BlobLinkerDefinitionGenerator& generator) {
		generator.setPathPrefix(pathPrefix);
		generator.setFileSuffix(fileSuffix);
	}

	// ======== BlobAliasLanguage ========

	BlobAliasLanguage::BlobAliasLanguage() : FileConversionLanguage("blob aliases") {}

	BlobAliasLanguage::BlobAliasLanguage(const BlobAliasLanguage& language) : FileConversionLanguage(language) {}

	string BlobAliasLanguage::getTargetBasename(const string& sourceBasename,
			const Flavor&, const Flavor&, Component&) {
		return sourceBasename + ".cpp";
	}

	Transform* BlobAliasLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor& targetFlavor,
			const Flavor& transformFlavor, Component& component, BuildContext&) {
		Unref<AliasConfiguration> configuration(getAliasConfiguration(sourceArtifact,
				sourceFlavor, targetArtifact, targetFlavor, transformFlavor, component));
		return new BlobAliasGenerationTransform(sourceArtifact, *configuration);
	}

	BlobAliasLanguage::AliasConfiguration* BlobAliasLanguage::getAliasConfiguration(FileArtifact&, const Flavor&,
			FileArtifact&, const Flavor&, const Flavor&, Component&) {
		return NULL;
	}

	static const char *const SOURCE_SUFFIXES[] = {
		".rbln",
		NULL
	};

	Language::ArtifactType BlobAliasLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, NULL);
	}

}}

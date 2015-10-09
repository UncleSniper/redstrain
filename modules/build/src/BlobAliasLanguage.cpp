#include <redstrain/util/Unref.hpp>

#include "BlobAliasLanguage.hpp"
#include "BlobAliasGenerationTransform.hpp"

using std::string;
using redengine::util::Unref;

namespace redengine {
namespace build {

	// ======== AliasConfiguration ========

	BlobAliasLanguage::AliasConfiguration::AliasConfiguration() {}

	BlobAliasLanguage::AliasConfiguration::AliasConfiguration(const string& pathPrefix, const string& fileSuffix)
			: pathPrefix(pathPrefix), fileSuffix(fileSuffix) {}

	BlobAliasLanguage::AliasConfiguration::AliasConfiguration(const AliasConfiguration& configuration)
			: ReferenceCounted(configuration), pathPrefix(configuration.pathPrefix),
			fileSuffix(configuration.fileSuffix) {}

	BlobAliasLanguage::AliasConfiguration::~AliasConfiguration() {}

	void BlobAliasLanguage::AliasConfiguration::setPathPrefix(const string& prefix) {
		pathPrefix = prefix;
	}

	void BlobAliasLanguage::AliasConfiguration::setFileSuffix(const string& suffix) {
		fileSuffix = suffix;
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
			const Flavor& transformFlavor, Component& component) {
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

#include <redstrain/util/Unref.hpp>

#include "BlobLanguage.hpp"
#include "BlobGenerationTransform.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Appender;

namespace redengine {
namespace build {

	// ======== BlobConfiguration ========

	BlobLanguage::BlobConfiguration::BlobConfiguration() {}

	BlobLanguage::BlobConfiguration::BlobConfiguration(const BlobConfiguration& configuration)
			: ReferenceCounted(configuration) {}

	BlobLanguage::BlobConfiguration::~BlobConfiguration() {}

	// ======== BlobLanguage ========

	BlobLanguage::BlobLanguage() : FileConversionLanguage("blob") {}

	BlobLanguage::BlobLanguage(const BlobLanguage& language) : FileConversionLanguage(language) {}

	string BlobLanguage::getTargetBasename(const string& sourceBasename, const Flavor&,
			const Flavor& transformFlavor, Component&) {
		if(transformFlavor == Flavor::HEADER)
			return sourceBasename + ".hpp";
		else
			return sourceBasename + ".cpp";
	}

	Transform* BlobLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, const Flavor& transformFlavor, Component& component) {
		Unref<BlobConfiguration> configuration(getBlobConfiguration(sourceArtifact,
				sourceFlavor, transformFlavor, component));
		return new BlobGenerationTransform(sourceArtifact, *configuration, transformFlavor == Flavor::HEADER);
	}

	BlobLanguage::BlobConfiguration* BlobLanguage::getBlobConfiguration(FileArtifact&, const Flavor&,
			const Flavor&, Component&) {
		return NULL;
	}

	static const char *const SOURCE_SUFFIXES[] = {
		".redct",
		NULL
	};

	Language::ArtifactType BlobLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, NULL);
	}

	static const Flavor *const FLAVORS[] = {
		&Flavor::HEADER,
		&Flavor::SOURCE,
		NULL
	};

	void BlobLanguage::getSupportedFlavors(Component::Type, Appender<Flavor>& sink) {
		Language::sinkSupportedFlavors(FLAVORS, sink);
	}

}}

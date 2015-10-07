#include <redstrain/util/Unref.hpp>

#include "BlobLanguage.hpp"
#include "BlobGenerationTransform.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Appender;
using redengine::io::CPPArrayOutputStream;

namespace redengine {
namespace build {

	// ======== BlobConfiguration ========

	BlobLanguage::BlobConfiguration::BlobConfiguration() {}

	BlobLanguage::BlobConfiguration::BlobConfiguration(const BlobConfiguration& configuration)
			: ReferenceCounted(configuration) {}

	BlobLanguage::BlobConfiguration::~BlobConfiguration() {}

	// ======== GenericBlobConfiguration ========

	BlobLanguage::GenericBlobConfiguration::GenericBlobConfiguration(const string& variable,
			const string& exportMacro, const string& blobPath, const string& extraInclude,
			const string& guardMacro) : variable(variable), exportMacro(exportMacro), blobPath(blobPath),
			extraInclude(extraInclude), guardMacro(guardMacro) {}

	BlobLanguage::GenericBlobConfiguration::GenericBlobConfiguration(const GenericBlobConfiguration& configuration)
			: BlobConfiguration(configuration), variable(configuration.variable),
			exportMacro(configuration.exportMacro), blobPath(configuration.blobPath),
			extraInclude(configuration.extraInclude), guardMacro(configuration.guardMacro) {}

	void BlobLanguage::GenericBlobConfiguration::setVariableName(const string& variable) {
		this->variable = variable;
	}

	void BlobLanguage::GenericBlobConfiguration::setExportMacro(const string& exportMacro) {
		this->exportMacro = exportMacro;
	}

	void BlobLanguage::GenericBlobConfiguration::setBlobPath(const string& blobPath) {
		this->blobPath = blobPath;
	}

	void BlobLanguage::GenericBlobConfiguration::setExtraInclude(const string& extraInclude) {
		this->extraInclude = extraInclude;
	}

	void BlobLanguage::GenericBlobConfiguration::setGuardMacro(const string& guardMacro) {
		this->guardMacro = guardMacro;
	}

	void BlobLanguage::GenericBlobConfiguration::applyConfiguration(CPPArrayOutputStream& stream) {
		stream.setVariableName(variable);
		stream.setExportMacro(exportMacro);
		stream.setBlobPath(blobPath);
		stream.setExtraInclude(extraInclude);
		stream.setGuardMacro(guardMacro);
	}

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
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor& targetFlavor,
			const Flavor& transformFlavor, Component& component) {
		Unref<BlobConfiguration> configuration(getBlobConfiguration(sourceArtifact,
				sourceFlavor, targetArtifact, targetFlavor, transformFlavor, component));
		return new BlobGenerationTransform(sourceArtifact, *configuration, transformFlavor == Flavor::HEADER);
	}

	BlobLanguage::BlobConfiguration* BlobLanguage::getBlobConfiguration(FileArtifact&, const Flavor&,
			FileArtifact&, const Flavor&, const Flavor&, Component&) {
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

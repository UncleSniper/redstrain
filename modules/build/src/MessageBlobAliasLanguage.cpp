#include <redstrain/util/Unref.hpp>

#include "MessageBlobAliasGenerationTransform.hpp"

using std::string;
using redengine::util::Unref;

namespace redengine {
namespace build {

	// ======== AliasConfiguration ========

	MessageBlobAliasLanguage::AliasConfiguration::AliasConfiguration(const string& mappingSymbol,
			const string& blobNSPrefix) : mappingSymbol(mappingSymbol), blobNSPrefix(blobNSPrefix) {}

	MessageBlobAliasLanguage::AliasConfiguration::AliasConfiguration(const AliasConfiguration& configuration)
			: ReferenceCounted(configuration), mappingSymbol(configuration.mappingSymbol),
			blobNSPrefix(configuration.blobNSPrefix) {}

	void MessageBlobAliasLanguage::AliasConfiguration::setMappingSymbol(const string& symbol) {
		mappingSymbol = symbol;
	}

	void MessageBlobAliasLanguage::AliasConfiguration::setBlobNamespacePrefix(const string& prefix) {
		blobNSPrefix = prefix;
	}

	// ======== MessageBlobAliasLanguage ========

	MessageBlobAliasLanguage::MessageBlobAliasLanguage() : FileConversionLanguage("message blob aliases") {}

	MessageBlobAliasLanguage::MessageBlobAliasLanguage(const MessageBlobAliasLanguage& language)
			: FileConversionLanguage(language) {}

	string MessageBlobAliasLanguage::getTargetBasename(const string& sourceBasename,
			const Flavor&, const Flavor&, Component&) {
		return sourceBasename + ".cpp";
	}

	Transform* MessageBlobAliasLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor& targetFlavor,
			const Flavor& transformFlavor, Component& component, BuildContext&) {
		Unref<AliasConfiguration> configuration(&getAliasConfiguration(sourceArtifact,
				sourceFlavor, targetArtifact, targetFlavor, transformFlavor, component));
		return new MessageBlobAliasGenerationTransform(sourceArtifact, **configuration);
	}

	static const char *const SOURCE_SUFFIXES[] = {
		".msgln",
		NULL
	};

	Language::ArtifactType MessageBlobAliasLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, NULL);
	}

}}

#include <redstrain/util/Unref.hpp>

#include "MessageBlobRegisterLanguage.hpp"

using std::string;
using redengine::util::Unref;

namespace redengine {
namespace build {

	// ======== RegistrarConfiguration ========

	MessageBlobRegisterLanguage::RegistrarConfiguration::RegistrarConfiguration(const string& mappingSymbol,
			const string& blobSymbol, const string& language, const string& country)
			: mappingSymbol(mappingSymbol), blobSymbol(blobSymbol), language(language), country(country) {}

	MessageBlobRegisterLanguage::RegistrarConfiguration::RegistrarConfiguration(const
			RegistrarConfiguration& configuration) : mappingSymbol(configuration.mappingSymbol),
			blobSymbol(configuration.blobSymbol), language(configuration.language), country(configuration.country) {}

	MessageBlobRegisterLanguage::RegistrarConfiguration::~RegistrarConfiguration() {}

	void MessageBlobRegisterLanguage::RegistrarConfiguration::setMappingSymbol(const string& symbol) {
		mappingSymbol = symbol;
	}

	void MessageBlobRegisterLanguage::RegistrarConfiguration::setBlobSymbol(const string& symbol) {
		blobSymbol = symbol;
	}

	void MessageBlobRegisterLanguage::RegistrarConfiguration::setLanguage(const string& language) {
		this->language = language;
	}

	void MessageBlobRegisterLanguage::RegistrarConfiguration::setCountry(const string& country) {
		this->country = country;
	}

	// ======== MessageBlobRegisterLanguage ========

	MessageBlobRegisterLanguage::MessageBlobRegisterLanguage() : FileConversionLanguage("blob message catalog") {}

	MessageBlobRegisterLanguage::MessageBlobRegisterLanguage(const MessageBlobRegisterLanguage& language)
			: FileConversionLanguage(language) {}

	string MessageBlobRegisterLanguage::getTargetBasename(const string& sourceBasename,
			const Flavor&, const Flavor&, Component&) {
		return sourceBasename + "-mbr.cpp";
	}

	Transform* MessageBlobRegisterLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor& sourceFlavor, FileArtifact& targetArtifact, const Flavor& targetFlavor,
			const Flavor& transformFlavor, Component& component, BuildContext&) {
		Unref<RegistrarConfiguration> configuration(&getRegistrarConfiguration(sourceArtifact,
				sourceFlavor, targetArtifact, targetFlavor, transformFlavor, component));
		//TODO
		return NULL;
	}

	static const char *const SOURCE_SUFFIXES[] = {
		".rlmg2",
		NULL
	};

	Language::ArtifactType MessageBlobRegisterLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, NULL);
	}

}}

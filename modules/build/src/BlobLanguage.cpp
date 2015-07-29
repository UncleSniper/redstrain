#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>

#include "FileArtifact.hpp"
#include "BlobGenerationAction.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::util::Appender;
using redengine::util::StringUtils;
using redengine::io::CPPArrayOutputStream;

namespace redengine {
namespace build {

	// ======== BlobConfiguration ========

	BlobLanguage::BlobConfiguration::BlobConfiguration() {}

	BlobLanguage::BlobConfiguration::BlobConfiguration(const BlobConfiguration&) {}

	BlobLanguage::BlobConfiguration::~BlobConfiguration() {}

	// ======== EmptyBlobConfiguration ========

	BlobLanguage::EmptyBlobConfiguration BlobLanguage::EmptyBlobConfiguration::instance;

	BlobLanguage::EmptyBlobConfiguration::EmptyBlobConfiguration() {}

	BlobLanguage::EmptyBlobConfiguration::EmptyBlobConfiguration(const EmptyBlobConfiguration& configuration)
			: BlobConfiguration(configuration) {}

	void BlobLanguage::EmptyBlobConfiguration::applyConfiguration(CPPArrayOutputStream&) {}

	// ======== BlobLanguage ========

	BlobLanguage::BlobLanguage() : FileConversionLanguage("blob") {}

	BlobLanguage::BlobLanguage(const BlobLanguage& language) : FileConversionLanguage(language) {}

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

	string BlobLanguage::getTargetBasename(const string& sourceBasename, const Flavor&,
			const Flavor& transformFlavor, const Component&) {
		if(transformFlavor == Flavor::HEADER)
			return sourceBasename + ".hpp";
		else
			return sourceBasename + ".cpp";
	}

	GenerationAction<FileArtifact>* BlobLanguage::newGenerationAction(FileArtifact* target,
			const Flavor&, const Flavor& transformFlavor, const Component& component, BuildContext& context) {
		Unref<BlobGenerationAction> action(new BlobGenerationAction(target,
				getBlobConfiguration(transformFlavor, component), transformFlavor == Flavor::HEADER));
		action->addIntermediateDirectories(component, context);
		return action.set();
	}

}}

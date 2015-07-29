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

	BlobLanguage::BlobConfiguration::BlobConfiguration(const BlobConfiguration& configuration)
			: ReferenceCounted(configuration) {}

	BlobLanguage::BlobConfiguration::~BlobConfiguration() {}

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
		Unref<BlobConfiguration> configuration(getBlobConfiguration(*target, transformFlavor, component));
		Unref<BlobGenerationAction> action(new BlobGenerationAction(target, *configuration,
				transformFlavor == Flavor::HEADER));
		action->addIntermediateDirectories(component, context);
		return action.set();
	}

	BlobLanguage::BlobConfiguration* BlobLanguage::getBlobConfiguration(const FileArtifact&,
			const Flavor&, const Component&) {
		return NULL;
	}

}}

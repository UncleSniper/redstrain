#include <redstrain/util/Unref.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "BuildContext.hpp"
#include "FileConversionLanguage.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Appender;
using redengine::platform::Pathname;

namespace redengine {
namespace build {

	FileConversionLanguage::FileConversionLanguage(const string& name) : Language(name) {}

	FileConversionLanguage::FileConversionLanguage(const FileConversionLanguage& language) : Language(language) {}

	Flavor FileConversionLanguage::getTargetFlavor(const Flavor&, const Flavor& transformFlavor) {
		return transformFlavor;
	}

	bool FileConversionLanguage::isTargetFinal(FileArtifact&, const Flavor&, const Flavor&,
			FileArtifact&, const Flavor&, Component&) {
		return false;
	}

	bool FileConversionLanguage::acceptsSource(const Flavor&, const Flavor&) {
		return true;
	}

	static const Flavor *const FLAVORS[] = {
		&Flavor::GENERIC,
		NULL
	};

	void FileConversionLanguage::getSupportedFlavors(Component::Type, Appender<Flavor>& sink) {
		Language::sinkSupportedFlavors(FLAVORS, sink);
	}

	bool FileConversionLanguage::isOneToOne(const Flavor&) {
		return true;
	}

	FileArtifact* FileConversionLanguage::getSourceTransform(BuildContext& context, const string& sourceDirectory,
			FileArtifact& sourceArtifact, const Flavor& sourceFlavor, const string& targetDirectory,
			const Flavor& transformFlavor, Component& component, BuildArtifactMapper&,
			ManyToOneTransform<FileArtifact>*& manyTransform, Flavor& targetFlavor, bool& isFinal) {
		string sourceTail(Pathname::stripPrefix(sourceArtifact.getPath(), sourceDirectory));
		string targetBasename(getTargetBasename(Pathname::basename(sourceTail),
				sourceFlavor, transformFlavor, component));
		string targetTail(Pathname::join(Pathname::dirname(sourceTail, Pathname::LOGICAL), targetBasename));
		FileArtifact& target = context.internFileArtifact(Pathname::join(targetDirectory, targetTail), targetTail);
		targetFlavor = getTargetFlavor(sourceFlavor, transformFlavor);
		Unref<Transform> transform(getConversionTransform(sourceArtifact, sourceFlavor,
				target, targetFlavor, transformFlavor, component));
		transform->addPrerequisite(sourceArtifact);
		target.setGeneratingTransform(*transform);
		target.addIntermediateDirectories(context, component.getBaseDirectory());
		manyTransform = NULL;
		isFinal = isTargetFinal(sourceArtifact, sourceFlavor, transformFlavor, target, targetFlavor, component);
		target.ref();
		return &target;
	}

	FileArtifact* FileConversionLanguage::getHeaderExposeTransform(BuildContext&, const string&, FileArtifact&,
			const Flavor&, const string&, Component&, BuildArtifactMapper&, Flavor&) {
		return NULL;
	}

}}

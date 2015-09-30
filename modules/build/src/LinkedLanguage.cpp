#include <redstrain/util/Unref.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "Linker.hpp"
#include "BuildContext.hpp"
#include "LinkTransform.hpp"
#include "LinkedLanguage.hpp"
#include "BuildArtifactMapper.hpp"
#include "EmptyLinkerConfiguration.hpp"

using std::string;
using redengine::util::Unref;
using redengine::platform::Pathname;

namespace redengine {
namespace build {

	LinkedLanguage::LinkedLanguage(Linker& linker)
			: ObjectFileLanguage(linker.getObjectFileFormatName()), linker(linker) {}

	LinkedLanguage::LinkedLanguage(const LinkedLanguage& language)
			: ObjectFileLanguage(language), linker(language.linker) {}

	LinkerConfiguration& LinkedLanguage::getLinkerConfiguration(const Flavor&, const Component&) {
		return EmptyLinkerConfiguration::instance;
	}

	Language::ArtifactType LinkedLanguage::classifyFile(const string& path) {
		return linker.isObjectFile(path) ? AT_SOURCE : AT_NONE;
	}

	bool LinkedLanguage::acceptsSource(const Flavor& sourceFlavor, const Flavor& transformFlavor) {
		return sourceFlavor == transformFlavor;
	}

	FileArtifact* LinkedLanguage::getSourceTransform(BuildContext& context, const string&,
			FileArtifact& sourceArtifact, const Flavor&, const string& targetDirectory,
			const Flavor& transformFlavor, Component& component, BuildArtifactMapper& artifactMapper,
			ManyToOneTransform<FileArtifact>*& manyTransform, Flavor& targetFlavor, bool& isFinal) {
		Linkage::LinkMode mode;
		if(transformFlavor == Flavor::STATIC)
			mode = component.getType() == Component::EXECUTABLE
					? Linkage::STATIC_EXECUTABLE : Linkage::STATIC_LIBRARY;
		else if(transformFlavor == Flavor::DYNAMIC)
			mode = component.getType() == Component::EXECUTABLE
					? Linkage::DYNAMIC_EXECUTABLE : Linkage::DYNAMIC_LIBRARY;
		else
			return NULL;
		string targetBasename(artifactMapper.getTargetFileName(component, *this, transformFlavor));
		FileArtifact& targetArtifact = context.internFileArtifact(Pathname::tidy(Pathname::join(targetDirectory,
				targetBasename)), targetBasename);
		Unref<LinkTransform> transform(new LinkTransform(linker, mode,
				getLinkerConfiguration(transformFlavor, component)));
		transform->addSource(sourceArtifact);
		transform->addPrerequisite(sourceArtifact);
		targetArtifact.setGeneratingTransform(*transform);
		targetArtifact.addIntermediateDirectories(context, component.getBaseDirectory());
		manyTransform = *transform;
		targetFlavor = transformFlavor;
		isFinal = true;
		targetArtifact.ref();
		return &targetArtifact;
	}

}}

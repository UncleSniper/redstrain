#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "Component.hpp"
#include "BlobGenerationAction.hpp"

using redengine::platform::Pathname;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	BlobGenerationAction::BlobGenerationAction(FileArtifact* target, BlobLanguage::BlobConfiguration* configuration,
			bool header) : GenerationAction<FileArtifact>(generation, target), generation(configuration, header) {}

	BlobGenerationAction::BlobGenerationAction(const BlobGenerationAction& action)
			: Action(action), GenerationAction<FileArtifact>(generation, action.getTarget()),
			FileGeneratingAction(action), generation(action.generation) {}

	void BlobGenerationAction::addIntermediateDirectories(const Component& component, BuildContext& context) {
		FileArtifact* target = getTarget();
		if(target)
			addIntermediateDirectoriesFor(component.getBaseDirectory(),
					Pathname::stripPrefix(Pathname::dirname(target->getPathname()),
					component.getBaseDirectory()), context);
	}

	void BlobGenerationAction::perform(BuildContext& context) {
		createIntermediateDirectories();
		GenerationAction<FileArtifact>::perform(context);
	}

	void BlobGenerationAction::wouldPerform(BuildContext& context) {
		wouldCreateIntermediateDirectories(context);
		GenerationAction<FileArtifact>::wouldPerform(context);
	}

	void BlobGenerationAction::dumpAction(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "BlobGenerationAction " << this << " {" << endln << shift;
		dumpGenerationActionAspects(stream);
		dumpFileGeneratingActionAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}

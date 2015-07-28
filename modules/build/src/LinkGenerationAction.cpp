#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "Component.hpp"
#include "LinkGenerationAction.hpp"

using redengine::platform::Pathname;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	LinkGenerationAction::LinkGenerationAction(FileArtifact* target, Linker& linker, Linkage::LinkMode mode,
			LinkerConfiguration& configuration)
			: GenerationAction<FileArtifact>(generation, target), generation(linker, mode, configuration) {}

	LinkGenerationAction::LinkGenerationAction(const LinkGenerationAction& action)
			: Action(action), GenerationAction<FileArtifact>(action), FileGeneratingAction(action),
			generation(action.generation) {}

	void LinkGenerationAction::addIntermediateDirectories(const Component& component, BuildContext& context) {
		FileArtifact* target = getTarget();
		if(target)
			addIntermediateDirectoriesFor(component.getBaseDirectory(),
					Pathname::stripPrefix(Pathname::dirname(target->getPathname()),
					component.getBaseDirectory()), context);
	}

	void LinkGenerationAction::perform(BuildContext& context) {
		createIntermediateDirectories();
		GenerationAction<FileArtifact>::perform(context);
	}

	void LinkGenerationAction::wouldPerform(BuildContext& context) {
		wouldCreateIntermediateDirectories(context);
		GenerationAction<FileArtifact>::wouldPerform(context);
	}

	void LinkGenerationAction::dumpAction(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "LinkGenerationAction " << this << " {" << endln << shift;
		dumpGenerationActionAspects(stream);
		dumpFileGeneratingActionAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}

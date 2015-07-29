#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "Component.hpp"
#include "CodeTableCompileGenerationAction.hpp"

using redengine::platform::Pathname;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	CodeTableCompileGenerationAction::CodeTableCompileGenerationAction(FileArtifact* target)
			: GenerationAction<FileArtifact>(generation, target) {}

	CodeTableCompileGenerationAction::CodeTableCompileGenerationAction(const CodeTableCompileGenerationAction& action)
			: Action(action), GenerationAction<FileArtifact>(generation, action.getTarget()),
			FileGeneratingAction(action), generation(action.generation) {}

	void CodeTableCompileGenerationAction::addIntermediateDirectories(const Component& component,
			BuildContext& context) {
		FileArtifact* target = getTarget();
		if(target)
			addIntermediateDirectoriesFor(component.getBaseDirectory(),
					Pathname::stripPrefix(Pathname::dirname(target->getPathname()),
					component.getBaseDirectory()), context);
	}

	void CodeTableCompileGenerationAction::perform(BuildContext& context) {
		createIntermediateDirectories();
		GenerationAction<FileArtifact>::perform(context);
	}

	void CodeTableCompileGenerationAction::wouldPerform(BuildContext& context) {
		wouldCreateIntermediateDirectories(context);
		GenerationAction<FileArtifact>::wouldPerform(context);
	}

	void CodeTableCompileGenerationAction::dumpAction(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "CodeTableCompileGenerationAction " << this << " {" << endln << shift;
		dumpGenerationActionAspects(stream);
		dumpFileGeneratingActionAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}

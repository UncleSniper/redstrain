#include "LinkGenerationAction.hpp"

namespace redengine {
namespace build {

	LinkGenerationAction::LinkGenerationAction(FileArtifact* target, Linker& linker, Linkage::LinkMode mode,
			LinkerConfiguration& configuration)
			: GenerationAction<FileArtifact>(generation, target), generation(linker, mode, configuration) {}

	LinkGenerationAction::LinkGenerationAction(const LinkGenerationAction& action)
			: GenerationAction<FileArtifact>(action), generation(action.generation) {}

}}

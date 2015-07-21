#include "CompileGenerationAction.hpp"

namespace redengine {
namespace build {

	CompileGenerationAction::CompileGenerationAction(FileArtifact* target, Compiler& compiler,
			Compilation::CompileMode mode, CompilerConfiguration& configuration)
			: GenerationAction<FileArtifact>(generation, target), generation(compiler, mode, configuration) {}

	CompileGenerationAction::CompileGenerationAction(const CompileGenerationAction& action)
			: GenerationAction<FileArtifact>(action), generation(action.generation) {}

}}

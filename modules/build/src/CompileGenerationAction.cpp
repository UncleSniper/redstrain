#include <redstrain/platform/Pathname.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "Component.hpp"
#include "CompileGenerationAction.hpp"

using redengine::platform::Pathname;
#ifdef TESTING_REDSTRAIN_BUILD_API
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;
#endif /* TESTING_REDSTRAIN_BUILD_API */

namespace redengine {
namespace build {

	CompileGenerationAction::CompileGenerationAction(FileArtifact* target, Compiler& compiler,
			Compilation::CompileMode mode, CompilerConfiguration& configuration)
			: GenerationAction<FileArtifact>(generation, target), generation(compiler, mode, configuration) {}

	CompileGenerationAction::CompileGenerationAction(const CompileGenerationAction& action)
			: Action(action), GenerationAction<FileArtifact>(action), FileGeneratingAction(action),
			generation(action.generation) {}

	void CompileGenerationAction::addIntermediateDirectories(const Component& component, BuildContext& context) {
		FileArtifact* target = getTarget();
		if(target)
			addIntermediateDirectoriesFor(component.getBaseDirectory(),
					Pathname::stripPrefix(Pathname::dirname(target->getPathname()),
					component.getBaseDirectory()), context);
	}

	void CompileGenerationAction::perform(BuildContext& context) {
		createIntermediateDirectories();
		GenerationAction<FileArtifact>::perform(context);
	}

	void CompileGenerationAction::wouldPerform(BuildContext& context) {
		wouldCreateIntermediateDirectories();
		GenerationAction<FileArtifact>::wouldPerform(context);
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void CompileGenerationAction::dumpAction(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "CompileGenerationAction " << this << " {" << endln << shift;
		dumpGenerationActionAspects(stream);
		dumpFileGeneratingActionAspects(stream);
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}

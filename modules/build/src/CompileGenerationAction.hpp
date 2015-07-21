#ifndef REDSTRAIN_MOD_BUILD_COMPILEGENERATIONACTION_HPP
#define REDSTRAIN_MOD_BUILD_COMPILEGENERATIONACTION_HPP

#include "GenerationAction.hpp"
#include "CompileGeneration.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CompileGenerationAction : public GenerationAction<FileArtifact> {

	  private:
		CompileGeneration generation;

	  public:
		CompileGenerationAction(FileArtifact*, Compiler&, Compilation::CompileMode, CompilerConfiguration&);
		CompileGenerationAction(const CompileGenerationAction&);

		inline CompileGeneration& getCompileGeneration() {
			return generation;
		}

		inline const CompileGeneration& getCompileGeneration() const {
			return generation;
		}

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILEGENERATIONACTION_HPP */

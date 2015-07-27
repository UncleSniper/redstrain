#ifndef REDSTRAIN_MOD_BUILD_COMPILEGENERATIONACTION_HPP
#define REDSTRAIN_MOD_BUILD_COMPILEGENERATIONACTION_HPP

#include "GenerationAction.hpp"
#include "CompileGeneration.hpp"
#include "FileGeneratingAction.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CompileGenerationAction
			: public GenerationAction<FileArtifact>, public FileGeneratingAction {

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

		void addIntermediateDirectories(const Component&, BuildContext&);

		virtual void perform(BuildContext&);
		virtual void wouldPerform(BuildContext&);

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpAction(io::DefaultConfiguredOutputStream<char>::Stream&) const;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILEGENERATIONACTION_HPP */

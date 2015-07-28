#ifndef REDSTRAIN_MOD_BUILD_CODETABLECOMPILEGENERATIONACTION_HPP
#define REDSTRAIN_MOD_BUILD_CODETABLECOMPILEGENERATIONACTION_HPP

#include "GenerationAction.hpp"
#include "CodeTableCompileGeneration.hpp"
#include "FileGeneratingAction.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CodeTableCompileGenerationAction
			: public GenerationAction<FileArtifact>, public FileGeneratingAction {

	  private:
		CodeTableCompileGeneration generation;

	  public:
		CodeTableCompileGenerationAction(FileArtifact*);
		CodeTableCompileGenerationAction(const CodeTableCompileGenerationAction&);

		inline CodeTableCompileGeneration& getCodeTableCompileGeneration() {
			return generation;
		}

		inline const CodeTableCompileGeneration& getCodeTableCompileGeneration() const {
			return generation;
		}

		void addIntermediateDirectories(const Component&, BuildContext&);

		virtual void perform(BuildContext&);
		virtual void wouldPerform(BuildContext&);

		virtual void dumpAction(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CODETABLECOMPILEGENERATIONACTION_HPP */

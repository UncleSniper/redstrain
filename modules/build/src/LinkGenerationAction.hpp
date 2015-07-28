#ifndef REDSTRAIN_MOD_BUILD_LINKGENERATIONACTION_HPP
#define REDSTRAIN_MOD_BUILD_LINKGENERATIONACTION_HPP

#include "LinkGeneration.hpp"
#include "GenerationAction.hpp"
#include "FileGeneratingAction.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API LinkGenerationAction
			: public GenerationAction<FileArtifact>, FileGeneratingAction {

	  private:
		LinkGeneration generation;

	  public:
		LinkGenerationAction(FileArtifact*, Linker&, Linkage::LinkMode, LinkerConfiguration&);
		LinkGenerationAction(const LinkGenerationAction&);

		inline LinkGeneration& getLinkGeneration() {
			return generation;
		}

		inline const LinkGeneration& getLinkGeneration() const {
			return generation;
		}

		void addIntermediateDirectories(const Component&, BuildContext&);

		virtual void perform(BuildContext&);
		virtual void wouldPerform(BuildContext&);

		virtual void dumpAction(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LINKGENERATIONACTION_HPP */

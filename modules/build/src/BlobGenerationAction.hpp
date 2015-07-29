#ifndef REDSTRAIN_MOD_BUILD_BLOBGENERATIONACTION_HPP
#define REDSTRAIN_MOD_BUILD_BLOBGENERATIONACTION_HPP

#include "BlobGeneration.hpp"
#include "GenerationAction.hpp"
#include "FileGeneratingAction.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API BlobGenerationAction
			: public GenerationAction<FileArtifact>, public FileGeneratingAction {

	  private:
		BlobGeneration generation;

	  public:
		BlobGenerationAction(FileArtifact*, BlobLanguage::BlobConfiguration&, bool);
		BlobGenerationAction(const BlobGenerationAction&);

		inline BlobGeneration& getBlobGeneration() {
			return generation;
		}

		inline const BlobGeneration& getBlobGeneration() const {
			return generation;
		}

		void addIntermediateDirectories(const Component&, BuildContext&);

		virtual void perform(BuildContext&);
		virtual void wouldPerform(BuildContext&);

		virtual void dumpAction(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BLOBGENERATIONACTION_HPP */

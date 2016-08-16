#ifndef REDSTRAIN_MOD_BUILD_BLOBGENERATIONTRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_BLOBGENERATIONTRANSFORM_HPP

#include "FileArtifact.hpp"
#include "BlobLanguage.hpp"
#include "OneToOneTransform.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API BlobGenerationTransform : public OneToOneTransform<FileArtifact> {

	  private:
		BlobLanguage::BlobConfiguration *const configuration;
		const bool header;

	  public:
		BlobGenerationTransform(FileArtifact&, BlobLanguage::BlobConfiguration*, bool);
		BlobGenerationTransform(const BlobGenerationTransform&);
		virtual ~BlobGenerationTransform();

		inline BlobLanguage::BlobConfiguration* getBlobConfiguration() const {
			return configuration;
		}

		inline bool isHeader() const {
			return header;
		}

		virtual void perform(BuildContext&, Artifact&);
		virtual void wouldPerform(BuildContext&, Artifact&);
		virtual bool onlyModifiesTarget() const;

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BLOBGENERATIONTRANSFORM_HPP */

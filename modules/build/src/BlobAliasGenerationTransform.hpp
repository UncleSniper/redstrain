#ifndef REDSTRAIN_MOD_BUILD_BLOBALIASGENERATIONTRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_BLOBALIASGENERATIONTRANSFORM_HPP

#include "FileArtifact.hpp"
#include "BlobAliasLanguage.hpp"
#include "OneToOneTransform.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API BlobAliasGenerationTransform : public OneToOneTransform<FileArtifact> {

	  private:
		BlobAliasLanguage::AliasConfiguration *const configuration;

	  public:
		BlobAliasGenerationTransform(FileArtifact&, BlobAliasLanguage::AliasConfiguration*);
		BlobAliasGenerationTransform(const BlobAliasGenerationTransform&);
		virtual ~BlobAliasGenerationTransform();

		inline BlobAliasLanguage::AliasConfiguration* getAliasConfiguration() const {
			return configuration;
		}

		virtual void perform(BuildContext&, Artifact&);
		virtual void wouldPerform(BuildContext&, Artifact&);
		virtual bool onlyModifiesTarget() const;

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BLOBALIASGENERATIONTRANSFORM_HPP */

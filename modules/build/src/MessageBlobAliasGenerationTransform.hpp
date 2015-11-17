#ifndef REDSTRAIN_MOD_BUILD_MESSAGEBLOBALIASGENERATIONTRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_MESSAGEBLOBALIASGENERATIONTRANSFORM_HPP

#include "FileArtifact.hpp"
#include "MessageBlobAliasLanguage.hpp"
#include "OneToOneTransform.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API MessageBlobAliasGenerationTransform : public OneToOneTransform<FileArtifact> {

	  private:
		MessageBlobAliasLanguage::AliasConfiguration& configuration;

	  public:
		MessageBlobAliasGenerationTransform(FileArtifact&, MessageBlobAliasLanguage::AliasConfiguration&);
		MessageBlobAliasGenerationTransform(const MessageBlobAliasGenerationTransform&);
		virtual ~MessageBlobAliasGenerationTransform();

		inline MessageBlobAliasLanguage::AliasConfiguration& getAliasConfiguration() {
			return configuration;
		}

		inline const MessageBlobAliasLanguage::AliasConfiguration& getAliasConfiguration() const {
			return configuration;
		}

		virtual void perform(BuildContext&, Artifact&);
		virtual void wouldPerform(BuildContext&, Artifact&);

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_MESSAGEBLOBALIASGENERATIONTRANSFORM_HPP */

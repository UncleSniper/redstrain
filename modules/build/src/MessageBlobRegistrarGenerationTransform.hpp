#ifndef REDSTRAIN_MOD_BUILD_MESSAGEBLOBREGISTRARGENERATIONTRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_MESSAGEBLOBREGISTRARGENERATIONTRANSFORM_HPP

#include "FileArtifact.hpp"
#include "OneToOneTransform.hpp"
#include "MessageBlobRegisterLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API MessageBlobRegistrarGenerationTransform : public OneToOneTransform<FileArtifact> {

	  private:
		MessageBlobRegisterLanguage::RegistrarConfiguration& configuration;

	  public:
		MessageBlobRegistrarGenerationTransform(FileArtifact&, MessageBlobRegisterLanguage::RegistrarConfiguration&);
		MessageBlobRegistrarGenerationTransform(const MessageBlobRegistrarGenerationTransform&);
		virtual ~MessageBlobRegistrarGenerationTransform();

		inline MessageBlobRegisterLanguage::RegistrarConfiguration& getRegistrarConfiguration() {
			return configuration;
		}

		inline const MessageBlobRegisterLanguage::RegistrarConfiguration& getRegistrarConfiguration() const {
			return configuration;
		}

		virtual void perform(BuildContext&, Artifact&);
		virtual void wouldPerform(BuildContext&, Artifact&);

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_MESSAGEBLOBREGISTRARGENERATIONTRANSFORM_HPP */

#ifndef REDSTRAIN_MOD_BUILD_CODETABLE16REGISTRARGENERATIONTRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_CODETABLE16REGISTRARGENERATIONTRANSFORM_HPP

#include "FileArtifact.hpp"
#include "OneToOneTransform.hpp"
#include "CodeTable16RegisterLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CodeTable16RegistrarGenerationTransform : public OneToOneTransform<FileArtifact> {

	  private:
		CodeTable16RegisterLanguage::RegistrarConfiguration *const configuration;

	  public:
		CodeTable16RegistrarGenerationTransform(FileArtifact&, CodeTable16RegisterLanguage::RegistrarConfiguration*);
		CodeTable16RegistrarGenerationTransform(const CodeTable16RegistrarGenerationTransform&);
		virtual ~CodeTable16RegistrarGenerationTransform();

		inline CodeTable16RegisterLanguage::RegistrarConfiguration* getRegistrarConfiguration() const {
			return configuration;
		}

		virtual void perform(BuildContext&, Artifact&);
		virtual void wouldPerform(BuildContext&, Artifact&);

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CODETABLE16REGISTRARGENERATIONTRANSFORM_HPP */

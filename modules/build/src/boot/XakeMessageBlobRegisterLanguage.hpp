#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEMESSAGEBLOBREGISTERLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEMESSAGEBLOBREGISTERLANGUAGE_HPP

#include "../MessageBlobRegisterLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeMessageBlobRegisterLanguage : public MessageBlobRegisterLanguage {

	  private:
		const XakeProject& project;

	  protected:
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&, BuildContext&);
		virtual RegistrarConfiguration& getRegistrarConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);

	  public:
		XakeMessageBlobRegisterLanguage(const XakeProject&);
		XakeMessageBlobRegisterLanguage(const XakeMessageBlobRegisterLanguage&);

		inline const XakeProject& getProject() const {
			return project;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEMESSAGEBLOBREGISTERLANGUAGE_HPP */

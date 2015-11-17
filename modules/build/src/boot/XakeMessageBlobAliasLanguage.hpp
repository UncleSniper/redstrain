#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEMESSAGEBLOBALIASLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEMESSAGEBLOBALIASLANGUAGE_HPP

#include "../MessageBlobAliasLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeMessageBlobAliasLanguage : public MessageBlobAliasLanguage {

	  private:
		const XakeProject& project;

	  protected:
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&, BuildContext&);
		virtual AliasConfiguration& getAliasConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);

	  public:
		XakeMessageBlobAliasLanguage(const XakeProject&);
		XakeMessageBlobAliasLanguage(const XakeMessageBlobAliasLanguage&);

		inline const XakeProject& getProject() const {
			return project;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEMESSAGEBLOBALIASLANGUAGE_HPP */

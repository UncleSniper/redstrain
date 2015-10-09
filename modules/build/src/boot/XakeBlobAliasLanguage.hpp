#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEBLOBALIASLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEBLOBALIASLANGUAGE_HPP

#include "../BlobAliasLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	class REDSTRAIN_BUILD_API XakeBlobAliasLanguage : public BlobAliasLanguage {

	  private:
		const XakeProject& project;

	  protected:
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);
		virtual AliasConfiguration* getAliasConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);

	  public:
		XakeBlobAliasLanguage(const XakeProject&);
		XakeBlobAliasLanguage(const XakeBlobAliasLanguage&);

		inline const XakeProject& getProject() const {
			return project;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEBLOBALIASLANGUAGE_HPP */

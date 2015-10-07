#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEBLOBLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEBLOBLANGUAGE_HPP

#include "../BlobLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeBlobLanguage : public BlobLanguage {

	  private:
		const XakeProject& project;

	  protected:
		virtual BlobConfiguration* getBlobConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);

	  public:
		XakeBlobLanguage(const XakeProject&);
		XakeBlobLanguage(const XakeBlobLanguage&);

		inline const XakeProject& getProject() const {
			return project;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEBLOBLANGUAGE_HPP */

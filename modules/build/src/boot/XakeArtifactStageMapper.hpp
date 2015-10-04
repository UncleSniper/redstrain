#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEARTIFACTSTAGEMAPPER_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEARTIFACTSTAGEMAPPER_HPP

#include "../ArtifactStageMapper.hpp"

namespace redengine {
namespace build {
namespace boot {

	class REDSTRAIN_BUILD_API XakeArtifactStageMapper : public ArtifactStageMapper {

	  public:
		static const char *const DEFAULT_STAGE_DIRECTORY;

	  public:
		XakeArtifactStageMapper();
		XakeArtifactStageMapper(const XakeArtifactStageMapper&);

		virtual ArtifactStage* getArtifactStage(const Component&, const Language&, const Flavor&, const Flavor&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEARTIFACTSTAGEMAPPER_HPP */

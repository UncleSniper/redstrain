#ifndef REDSTRAIN_MOD_BUILD_ARTIFACTSTAGEMAPPER_HPP
#define REDSTRAIN_MOD_BUILD_ARTIFACTSTAGEMAPPER_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class Flavor;
	class Language;
	class Component;
	class ArtifactStage;

	class REDSTRAIN_BUILD_API ArtifactStageMapper {

	  public:
		ArtifactStageMapper();
		ArtifactStageMapper(const ArtifactStageMapper&);
		virtual ~ArtifactStageMapper();

		virtual ArtifactStage* getArtifactStage(const Component&, const Language&, const Flavor&, const Flavor&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ARTIFACTSTAGEMAPPER_HPP */

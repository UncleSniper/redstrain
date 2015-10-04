#include "XakeComponent.hpp"
#include "XakeArtifactStageMapper.hpp"

namespace redengine {
namespace build {
namespace boot {

	const char *const XakeArtifactStageMapper::DEFAULT_STAGE_DIRECTORY = "stage";

	XakeArtifactStageMapper::XakeArtifactStageMapper() {}

	XakeArtifactStageMapper::XakeArtifactStageMapper(const XakeArtifactStageMapper& mapper)
			: ArtifactStageMapper(mapper) {}

	ArtifactStage* XakeArtifactStageMapper::getArtifactStage(const Component& component,
			const Language&, const Flavor&, const Flavor&) {
		const XakeComponent* xcomponent = dynamic_cast<const XakeComponent*>(&component);
		if(xcomponent)
			return &xcomponent->getArtifactStage();
		else
			return NULL;
	}

}}}

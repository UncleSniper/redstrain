#include "Artifact.hpp"

namespace redengine {
namespace build {

	Artifact::Artifact() : predictedPresent(false), predictedTimestamp(static_cast<time_t>(0u)) {}

	Artifact::Artifact(const Artifact& artifact) : ReferenceCounted(artifact),
			predictedPresent(artifact.predictedPresent), predictedTimestamp(artifact.predictedTimestamp) {}

	bool Artifact::wouldBePresent() {
		return predictedPresent;
	}

	time_t Artifact::getPredictedModificationTimestamp() {
		return predictedTimestamp;
	}

	void Artifact::setPredictedModificationTimestamp(time_t timestamp) {
		predictedPresent = true;
		predictedTimestamp = timestamp;
	}

	void Artifact::wouldModify() {
		predictedTimestamp = time(NULL);
		predictedPresent = true;
	}

	void Artifact::wouldRemove() {
		predictedPresent = false;
	}

}}

#include "Artifact.hpp"

namespace redengine {
namespace build {

	Artifact::DefinitiveMood Artifact::DEFINITIVE_MOOD;

	Artifact::PredictiveMood Artifact::PREDICTIVE_MOOD;

	Artifact::Mood::Mood() {}

	Artifact::Mood::Mood(const Mood&) {}

	Artifact::Mood::~Mood() {}

	Artifact::DefinitiveMood::DefinitiveMood() {}

	Artifact::DefinitiveMood::DefinitiveMood(const DefinitiveMood& mood) : Mood(mood) {}

	bool Artifact::DefinitiveMood::present(Artifact& artifact) const {
		return artifact.isPresent();
	}

	time_t Artifact::DefinitiveMood::modificationTimestamp(Artifact& artifact) const {
		return artifact.getModificationTimestamp();
	}

	Artifact::PredictiveMood::PredictiveMood() {}

	Artifact::PredictiveMood::PredictiveMood(const PredictiveMood& mood) : Mood(mood) {}

	bool Artifact::PredictiveMood::present(Artifact& artifact) const {
		return artifact.wouldBePresent();
	}

	time_t Artifact::PredictiveMood::modificationTimestamp(Artifact& artifact) const {
		return artifact.getPredictedModificationTimestamp();
	}

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
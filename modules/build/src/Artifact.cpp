#include "Artifact.hpp"

using redengine::util::Appender;

namespace redengine {
namespace build {

	// ======== Mood ========

	Artifact::Mood::Mood() {}

	Artifact::Mood::Mood(const Mood&) {}

	Artifact::Mood::~Mood() {}

	// ======== DefinitiveMood ========

	Artifact::DefinitiveMood::DefinitiveMood() {}

	Artifact::DefinitiveMood::DefinitiveMood(const DefinitiveMood& mood) : Mood(mood) {}

	bool Artifact::DefinitiveMood::present(Artifact& artifact) const {
		return artifact.isPresent();
	}

	void Artifact::DefinitiveMood::modificationTimestamp(Artifact& artifact, Appender<time_t>& sink) const {
		artifact.getModificationTimestamp(sink);
	}

	void Artifact::DefinitiveMood::virtualClock(Artifact& artifact, Appender<time_t>& sink) const {
		artifact.getVirtualClock(sink);
	}

	void Artifact::DefinitiveMood::modified(Artifact& artifact, BuildContext& context) const {
		artifact.notifyModified(context);
	}

	void Artifact::DefinitiveMood::remove(Artifact& artifact) const {
		artifact.remove();
	}

	// ======== PredictiveMood ========

	Artifact::PredictiveMood::PredictiveMood() {}

	Artifact::PredictiveMood::PredictiveMood(const PredictiveMood& mood) : Mood(mood) {}

	bool Artifact::PredictiveMood::present(Artifact& artifact) const {
		return artifact.wouldBePresent();
	}

	void Artifact::PredictiveMood::modificationTimestamp(Artifact& artifact, Appender<time_t>& sink) const {
		artifact.getPredictedModificationTimestamp(sink);
	}

	void Artifact::PredictiveMood::virtualClock(Artifact& artifact, Appender<time_t>& sink) const {
		artifact.getPredictedVirtualClock(sink);
	}

	void Artifact::PredictiveMood::modified(Artifact& artifact, BuildContext& context) const {
		artifact.wouldModify(context);
	}

	void Artifact::PredictiveMood::remove(Artifact& artifact) const {
		artifact.wouldRemove();
	}

	// ======== Artifact ========

	Artifact::Artifact() {}

	Artifact::Artifact(const Artifact& artifact) : ReferenceCounted(artifact) {}

	Artifact::~Artifact() {}

}}

#include "AbstractArtifact.hpp"

using redengine::util::Appender;

namespace redengine {
namespace build {

	AbstractArtifact::AbstractArtifact() : predictedPresent(false), predictedTimestamp(static_cast<time_t>(0u)),
			predictedVirtualClock(static_cast<time_t>(0u)), virtualClock(static_cast<time_t>(0u)) {}

	AbstractArtifact::AbstractArtifact(const AbstractArtifact& artifact)
			: Artifact(artifact), predictedPresent(artifact.predictedPresent),
			predictedTimestamp(artifact.predictedTimestamp), predictedVirtualClock(artifact.predictedVirtualClock),
			virtualClock(artifact.virtualClock) {}

	void AbstractArtifact::setPredictedModificationTimestamp(time_t timestamp) {
		predictedPresent = true;
		predictedTimestamp = timestamp;
	}

	bool AbstractArtifact::wouldBePresent() {
		return predictedPresent;
	}

	void AbstractArtifact::getPredictedModificationTimestamp(Appender<time_t>& sink) {
		sink.append(predictedTimestamp);
		sink.doneAppending();
	}

	void AbstractArtifact::getPredictedVirtualClock(Appender<time_t>& sink) {
		sink.append(predictedVirtualClock);
		sink.doneAppending();
	}

	void AbstractArtifact::wouldModify(BuildContext&) {
		//TODO
	}

	void AbstractArtifact::wouldRemove() {
		predictedPresent = false;
	}

	void AbstractArtifact::getVirtualClock(Appender<time_t>& sink) {
		sink.append(virtualClock);
		sink.doneAppending();
	}

	void AbstractArtifact::notifyModified(BuildContext&) {
		//TODO
	}

}}

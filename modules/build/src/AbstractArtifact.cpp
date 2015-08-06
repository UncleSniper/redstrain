#include <redstrain/platform/Filesystem.hpp>

#include "BuildContext.hpp"
#include "ArtifactStage.hpp"
#include "AbstractArtifact.hpp"

using redengine::util::Appender;
using redengine::platform::Filesystem;

namespace redengine {
namespace build {

	AbstractArtifact::AbstractArtifact() : predictedPresent(false), predictedTimestamp(static_cast<time_t>(0u)),
			predictedVirtualClock(static_cast<time_t>(0u)), virtualClock(static_cast<time_t>(0u)), stage(NULL) {}

	AbstractArtifact::AbstractArtifact(const AbstractArtifact& artifact)
			: Artifact(artifact), predictedPresent(artifact.predictedPresent),
			predictedTimestamp(artifact.predictedTimestamp), predictedVirtualClock(artifact.predictedVirtualClock),
			virtualClock(artifact.virtualClock), stage(artifact.stage) {
		if(stage)
			stage->ref();
	}

	AbstractArtifact::~AbstractArtifact() {
		if(stage)
			stage->unref();
	}

	void AbstractArtifact::setStage(ArtifactStage* stage) {
		if(stage)
			stage->ref();
		if(this->stage)
			this->stage->unref();
		this->stage = stage;
	}

	static ArtifactStage defaultStage(Filesystem::getSystemTempDirectory());

	ArtifactStage& AbstractArtifact::getEffectiveArtifactStage() const {
		return stage ? *stage : defaultStage;
	}

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

	void AbstractArtifact::wouldModify(BuildContext& context) {
		predictedTimestamp = time(NULL);
		predictedVirtualClock = context.tickVirtualTime();
		predictedPresent = true;
	}

	void AbstractArtifact::wouldRemove() {
		predictedPresent = false;
	}

	void AbstractArtifact::getVirtualClock(Appender<time_t>& sink) {
		sink.append(virtualClock);
		sink.doneAppending();
	}

	void AbstractArtifact::notifyModified(BuildContext& context) {
		virtualClock = context.tickVirtualTime();
	}

}}

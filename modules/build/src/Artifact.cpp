#include <redstrain/util/MinimumAppender.hpp>
#include <redstrain/util/MaximumAppender.hpp>

#include "Artifact.hpp"
#include "Transform.hpp"
#include "NoGeneratingTransformError.hpp"

using redengine::util::Appender;
using redengine::util::MinimumAppender;
using redengine::util::MaximumAppender;

namespace redengine {
namespace build {

	// ======== Mood ========

	Artifact::Mood::Mood() {}

	Artifact::Mood::Mood(const Mood&) {}

	Artifact::Mood::~Mood() {}

	// ======== DefinitiveMood ========

	Artifact::DefinitiveMood Artifact::DefinitiveMood::instance;

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

	void Artifact::DefinitiveMood::require(Artifact& artifact, BuildContext& context) const {
		artifact.require(context);
	}

	void Artifact::DefinitiveMood::perform(Transform& transform, BuildContext& context) const {
		transform.perform(context);
	}

	// ======== PredictiveMood ========

	Artifact::PredictiveMood Artifact::PredictiveMood::instance;

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

	void Artifact::PredictiveMood::require(Artifact& artifact, BuildContext& context) const {
		artifact.wouldRequire(context);
	}

	void Artifact::PredictiveMood::perform(Transform& transform, BuildContext& context) const {
		transform.wouldPerform(context);
	}

	// ======== Artifact ========

	Artifact::Artifact() {}

	Artifact::Artifact(const Artifact& artifact) : ReferenceCounted(artifact),
			generatingTransform(artifact.generatingTransform) {
		if(generatingTransform)
			generatingTransform->ref();
	}

	Artifact::~Artifact() {
		if(generatingTransform)
			generatingTransform->unref();
	}

	void Artifact::setGeneratingTransform(Transform* transform) {
		if(transform)
			transform->ref();
		if(generatingTransform)
			generatingTransform->unref();
		generatingTransform = transform;
	}

	void Artifact::require(const Mood& mood, BuildContext& context) {
		if(!mood.present(*this)) {
			if(!generatingTransform)
				throw NoGeneratingTransformError(*this);
			rebuild(mood, context);
			return;
		}
		if(!generatingTransform)
			return;
		MaximumAppender<time_t> newestSource, virtualNewestSource;
		Transform::PrerequisiteIterator pqbegin, pqend;
		generatingTransform->getPrerequisites(pqbegin, pqend);
		for(; pqbegin != pqend; ++pqbegin) {
			mood.require(**pqbegin, context);
			mood.modificationTimestamp(**pqbegin, newestSource);
			mood.virtualClock(**pqbegin, virtualNewestSource);
		}
		MinimumAppender<time_t> oldestTarget, virtualOldestTarget;
		mood.modificationTimestamp(*this, oldestTarget);
		mood.virtualClock(*this, virtualOldestTarget);
		if(
			newestSource.getMaximum() > oldestTarget.getMinimum()
			|| virtualNewestSource.getMaximum() > virtualOldestTarget.getMinimum()
		)
			rebuild(mood, context);
	}

	void Artifact::require(BuildContext& context) {
		require(DefinitiveMood::instance, context);
	}

	void Artifact::wouldRequire(BuildContext& context) {
		require(PredictiveMood::instance, context);
	}

	void Artifact::rebuild(const Mood& mood, BuildContext& context) {
		mood.perform(*generatingTransform, context);
		mood.modified(*this, context);
	}

}}

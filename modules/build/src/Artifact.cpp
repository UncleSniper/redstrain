#include <redstrain/util/MinimumAppender.hpp>
#include <redstrain/util/MaximumAppender.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "Artifact.hpp"
#include "Transform.hpp"
#include "NoGeneratingTransformError.hpp"

using redengine::util::Appender;
using redengine::util::MinimumAppender;
using redengine::util::MaximumAppender;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

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

	void Artifact::DefinitiveMood::perform(Transform& transform, BuildContext& context, Artifact& target) const {
		transform.perform(context, target);
	}

	bool Artifact::DefinitiveMood::isStaticallyUpToDate(const Artifact& artifact) const {
		return !!(artifact.artifactFlags & Artifact::AFL_DEFINITIVE_STATICALLY_UP_TO_DATE);
	}

	void Artifact::DefinitiveMood::markAsStaticallyUpToDate(Artifact& artifact) const {
		artifact.artifactFlags |= Artifact::AFL_DEFINITIVE_STATICALLY_UP_TO_DATE;
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

	void Artifact::PredictiveMood::perform(Transform& transform, BuildContext& context, Artifact& target) const {
		transform.wouldPerform(context, target);
	}

	bool Artifact::PredictiveMood::isStaticallyUpToDate(const Artifact& artifact) const {
		return !!(artifact.artifactFlags & Artifact::AFL_PREDICTIVE_STATICALLY_UP_TO_DATE);
	}

	void Artifact::PredictiveMood::markAsStaticallyUpToDate(Artifact& artifact) const {
		artifact.artifactFlags |= Artifact::AFL_PREDICTIVE_STATICALLY_UP_TO_DATE;
	}

	// ======== FollowupTransformPropertyInjector ========

	Artifact::FollowupTransformPropertyInjector::FollowupTransformPropertyInjector() {}

	Artifact::FollowupTransformPropertyInjector::FollowupTransformPropertyInjector(const
			FollowupTransformPropertyInjector& injector) : ReferenceCounted(injector) {}

	// ======== Artifact ========

	Artifact::Artifact() : generatingTransform(NULL), artifactFlags(0) {}

	Artifact::Artifact(const Artifact& artifact) : ReferenceCounted(artifact),
			generatingTransform(artifact.generatingTransform),
			followupTransformPropertyInjectors(artifact.followupTransformPropertyInjectors),
			artifactFlags(artifact.artifactFlags) {
		if(generatingTransform)
			generatingTransform->ref();
		FollowupTransformPropertyInjectorIterator ftpibegin(followupTransformPropertyInjectors.begin()),
				ftpiend(followupTransformPropertyInjectors.end());
		for(; ftpibegin != ftpiend; ++ftpibegin)
			(*ftpibegin)->ref();
	}

	Artifact::~Artifact() {
		if(generatingTransform)
			generatingTransform->unref();
		FollowupTransformPropertyInjectorIterator ftpibegin(followupTransformPropertyInjectors.begin()),
				ftpiend(followupTransformPropertyInjectors.end());
		for(; ftpibegin != ftpiend; ++ftpibegin)
			(*ftpibegin)->unref();
	}

	void Artifact::setGeneratingTransform(Transform* transform) {
		if(transform)
			transform->ref();
		if(generatingTransform)
			generatingTransform->unref();
		generatingTransform = transform;
	}

	bool Artifact::addFollowupTransformPropertyInjector(FollowupTransformPropertyInjector& injector) {
		if(!followupTransformPropertyInjectors.insert(&injector).second)
			return false;
		injector.ref();
		return true;
	}

	bool Artifact::removeFollowupTransformPropertyInjector(FollowupTransformPropertyInjector& injector) {
		if(!followupTransformPropertyInjectors.erase(&injector))
			return false;
		injector.unref();
		return true;
	}

	void Artifact::clearFollowupTransformPropertyInjectors() {
		FollowupTransformPropertyInjectorIterator ftpibegin(followupTransformPropertyInjectors.begin()),
				ftpiend(followupTransformPropertyInjectors.end());
		for(; ftpibegin != ftpiend; ++ftpibegin)
			(*ftpibegin)->unref();
		followupTransformPropertyInjectors.clear();
	}

	void Artifact::getFollowupTransformPropertyInjectors(FollowupTransformPropertyInjectorIterator& begin,
			FollowupTransformPropertyInjectorIterator& end) const {
		begin = followupTransformPropertyInjectors.begin();
		end = followupTransformPropertyInjectors.end();
	}

	void Artifact::injectFollowupTransformProperties(Component& component, Language& language,
			const Flavor& sourceFlavor, const Flavor& transformFlavor, Transform& transform) {
		FollowupTransformPropertyInjectorIterator ftpibegin(followupTransformPropertyInjectors.begin()),
				ftpiend(followupTransformPropertyInjectors.end());
		for(; ftpibegin != ftpiend; ++ftpibegin)
			(*ftpibegin)->injectFollowupTransformProperties(component, language,
					*this, sourceFlavor, transformFlavor, transform);
	}

	void Artifact::require(const Mood& mood, BuildContext& context) {
		if(mood.isStaticallyUpToDate(*this))
			return;
		Transform::PrerequisiteIterator pqbegin, pqend;
		if(generatingTransform) {
			generatingTransform->getPrerequisites(pqbegin, pqend);
			for(; pqbegin != pqend; ++pqbegin)
				mood.require(**pqbegin, context);
		}
		if(!mood.present(*this)) {
			if(!generatingTransform)
				throw NoGeneratingTransformError(*this);
			rebuild(mood, context);
			if(generatingTransform->onlyModifiesTarget())
				mood.markAsStaticallyUpToDate(*this);
			return;
		}
		if(!generatingTransform)
			return;
		MaximumAppender<time_t> newestSource, virtualNewestSource;
		generatingTransform->getPrerequisites(pqbegin, pqend);
		for(; pqbegin != pqend; ++pqbegin) {
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
		if(generatingTransform->onlyModifiesTarget())
			mood.markAsStaticallyUpToDate(*this);
	}

	void Artifact::require(BuildContext& context) {
		require(DefinitiveMood::instance, context);
	}

	void Artifact::wouldRequire(BuildContext& context) {
		require(PredictiveMood::instance, context);
	}

	void Artifact::rebuild(const Mood& mood, BuildContext& context) {
		mood.perform(*generatingTransform, context, *this);
		mood.modified(*this, context);
	}

	void Artifact::dumpArtifactAspects(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "generatingTransform =";
		if(generatingTransform) {
			stream << endln << shift;
			generatingTransform->dumpTransform(stream);
			stream << unshift;
		}
		else
			stream << " NULL" << endln;
	}

}}

#include "GenerationTrigger.hpp"

using std::list;

namespace redengine {
namespace build {

	GenerationTrigger::GenerationTrigger() {}

	struct ArtifactListUnref {

		list<Artifact*>* artifacts;

		ArtifactListUnref(list<Artifact*>* artifacts) : artifacts(artifacts) {}

		~ArtifactListUnref() {
			if(!artifacts)
				return;
			GenerationTrigger::ArtifactIterator begin(artifacts->begin()), end(artifacts->end());
			for(; begin != end; ++begin)
				(*begin)->unref();
		}

	};

	GenerationTrigger::GenerationTrigger(const GenerationTrigger& trigger) : Trigger(trigger) {
		ArtifactListUnref unrefSources(&sources);
		ArtifactIterator begin(trigger.sources.begin()), end(trigger.sources.end());
		for(; begin != end; ++begin) {
			sources.push_back(*begin);
			(*begin)->ref();
		}
		ArtifactListUnref unrefTargets(&targets);
		begin = trigger.targets.begin();
		end = trigger.targets.end();
		for(; begin != end; ++begin) {
			targets.push_back(*begin);
			(*begin)->ref();
		}
		unrefSources.artifacts = unrefTargets.artifacts = NULL;
	}

	GenerationTrigger::~GenerationTrigger() {
		ArtifactIterator begin(sources.begin()), end(sources.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		begin = targets.begin();
		end = targets.end();
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	void GenerationTrigger::addSource(Artifact* source) {
		if(!source)
			return;
		sources.push_back(source);
		source->ref();
	}

	bool GenerationTrigger::removeSource(Artifact* source) {
		if(!source)
			return false;
		list<Artifact*>::iterator begin(sources.begin()), end(sources.end());
		for(; begin != end; ++begin) {
			if(*begin == source) {
				sources.erase(begin);
				source->unref();
				return true;
			}
		}
		return false;
	}

	void GenerationTrigger::clearSources() {
		ArtifactIterator begin(sources.begin()), end(sources.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		sources.clear();
	}

	void GenerationTrigger::getSources(ArtifactIterator& begin, ArtifactIterator& end) const {
		begin = sources.begin();
		end = sources.end();
	}

	void GenerationTrigger::addTarget(Artifact* target) {
		if(!target)
			return;
		targets.push_back(target);
		target->ref();
	}

	bool GenerationTrigger::removeTarget(Artifact* target) {
		if(!target)
			return false;
		list<Artifact*>::iterator begin(targets.begin()), end(targets.end());
		for(; begin != end; ++begin) {
			if(*begin == target) {
				targets.erase(begin);
				target->unref();
				return true;
			}
		}
		return false;
	}

	void GenerationTrigger::clearTargets() {
		ArtifactIterator begin(targets.begin()), end(targets.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		targets.clear();
	}

	void GenerationTrigger::getTargets(ArtifactIterator& begin, ArtifactIterator& end) const {
		begin = targets.begin();
		end = targets.end();
	}

	bool GenerationTrigger::triggered(const Artifact::Mood& mood) const {
		if(sources.empty() || targets.empty())
			return false;
		time_t oldestTarget;
		bool hasTargets = false, missingTargets = false;
		ArtifactIterator begin(targets.begin()), end(targets.end());
		for(; begin != end; ++begin) {
			if(mood.present(**begin)) {
				time_t timestamp = mood.modificationTimestamp(**begin);
				if(hasTargets) {
					if(timestamp < oldestTarget)
						oldestTarget = timestamp;
				}
				else {
					oldestTarget = timestamp;
					hasTargets = true;
				}
			}
			else
				missingTargets = true;
		}
		time_t newestSource;
		bool hasSources = false, missingSources = false;
		begin = sources.begin();
		end = sources.end();
		for(; begin != end; ++begin) {
			if(mood.present(**begin)) {
				time_t timestamp = mood.modificationTimestamp(**begin);
				if(hasSources) {
					if(timestamp > newestSource)
						newestSource = timestamp;
				}
				else {
					newestSource = timestamp;
					hasSources = true;
				}
			}
			else
				missingSources = true;
		}
		if(missingSources)
			return false;
		return missingTargets || newestSource > oldestTarget;
	}

	bool GenerationTrigger::isTriggered() {
		return triggered(Artifact::DEFINITIVE_MOOD);
	}

	bool GenerationTrigger::wouldTrigger() {
		return triggered(Artifact::PREDICTIVE_MOOD);
	}

}}

#include <redstrain/io/streamoperators.hpp>

#include "GenerationTrigger.hpp"

using std::list;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	GenerationTrigger::GenerationTrigger() {}

	GenerationTrigger::GenerationTrigger(const GenerationTrigger& trigger)
			: Trigger(trigger), sources(trigger.sources), targets(trigger.targets),
			optionalSources(trigger.optionalSources) {
		ArtifactIterator begin(sources.begin()), end(sources.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
		begin = targets.begin();
		end = targets.end();
		for(; begin != end; ++begin)
			(*begin)->ref();
		begin = optionalSources.begin();
		end = optionalSources.end();
		for(; begin != end; ++begin)
			(*begin)->ref();
	}

	GenerationTrigger::~GenerationTrigger() {
		ArtifactIterator begin(sources.begin()), end(sources.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		begin = targets.begin();
		end = targets.end();
		for(; begin != end; ++begin)
			(*begin)->unref();
		begin = optionalSources.begin();
		end = optionalSources.end();
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

	void GenerationTrigger::addOptionalSource(Artifact* source) {
		if(!source)
			return;
		optionalSources.push_back(source);
		source->ref();
	}

	bool GenerationTrigger::removeOptionalSource(Artifact* source) {
		if(!source)
			return false;
		list<Artifact*>::iterator begin(optionalSources.begin()), end(optionalSources.end());
		for(; begin != end; ++begin) {
			if(*begin == source) {
				optionalSources.erase(begin);
				source->unref();
				return true;
			}
		}
		return false;
	}

	void GenerationTrigger::clearOptionalSources() {
		ArtifactIterator begin(optionalSources.begin()), end(optionalSources.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		optionalSources.clear();
	}

	void GenerationTrigger::getOptionalSources(ArtifactIterator& begin, ArtifactIterator& end) const {
		begin = optionalSources.begin();
		end = optionalSources.end();
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
		if((sources.empty() && optionalSources.empty()) || targets.empty())
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
		begin = optionalSources.begin();
		end = optionalSources.end();
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
		}
		if(missingSources)
			return false;
		return missingTargets || (hasSources && newestSource > oldestTarget);
	}

	bool GenerationTrigger::isTriggered(BuildContext&) {
		return triggered(Artifact::DEFINITIVE_MOOD);
	}

	bool GenerationTrigger::wouldTrigger(BuildContext&) {
		return triggered(Artifact::PREDICTIVE_MOOD);
	}

	void GenerationTrigger::dumpTrigger(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "GenerationTrigger " << this << " {" << endln << shift;
		stream << indent << "sources = {" << endln << shift;
		ArtifactIterator abegin(sources.begin()), aend(sources.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
		stream << indent << "optionalSources = {" << endln << shift;
		abegin = optionalSources.begin();
		aend = optionalSources.end();
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
		stream << indent << "targets = {" << endln << shift;
		abegin = targets.begin();
		aend = targets.end();
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
		dumpTriggerAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}

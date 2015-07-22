#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "PresenceTrigger.hpp"

using std::list;
#ifdef TESTING_REDSTRAIN_BUILD_API
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;
#endif /* TESTING_REDSTRAIN_BUILD_API */

namespace redengine {
namespace build {

	PresenceTrigger::PresenceTrigger(Junctor junctor) : junctor(junctor) {}

	PresenceTrigger::PresenceTrigger(const PresenceTrigger& trigger)
			: Trigger(trigger), junctor(trigger.junctor), artifacts(trigger.artifacts) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
	}

	PresenceTrigger::~PresenceTrigger() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	bool PresenceTrigger::addArtifact(Artifact* artifact) {
		if(!artifact)
			return false;
		if(!artifacts.insert(artifact).second)
			return false;
		artifact->ref();
		return true;
	}

	bool PresenceTrigger::removeArtifact(Artifact* artifact) {
		if(!artifacts.erase(artifact))
			return false;
		artifact->unref();
		return true;
	}

	void PresenceTrigger::clearArtifacts() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		artifacts.clear();
	}

	void PresenceTrigger::getArtifacts(ArtifactIterator& begin, ArtifactIterator& end) const {
		begin = artifacts.begin();
		end = artifacts.end();
	}

	bool PresenceTrigger::triggered(const Artifact::Mood& mood) const {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin) {
			bool present = mood.present(**begin);
			if(junctor == ANY) {
				if(present)
					return true;
			}
			else {
				if(!present)
					return false;
			}
		}
		return junctor != ANY;
	}

	bool PresenceTrigger::isTriggered(BuildContext&) {
		return triggered(Artifact::DEFINITIVE_MOOD);
	}

	bool PresenceTrigger::wouldTrigger(BuildContext&) {
		return triggered(Artifact::PREDICTIVE_MOOD);
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void PresenceTrigger::dumpTrigger(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "PresenceTrigger " << this << " {" << endln << shift;
		stream << indent << "junctor = " << (junctor == ANY ? "ANY" : "ALL") << endln;
		stream << indent << "artifacts = {" << endln << shift;
		ArtifactIterator abegin(artifacts.begin()), aend(artifacts.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
		dumpTriggerAspects(stream);
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}

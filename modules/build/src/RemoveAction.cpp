#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "Artifact.hpp"
#include "RemoveAction.hpp"

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

	RemoveAction::RemoveAction() {}

	RemoveAction::RemoveAction(const RemoveAction& action) : Action(action), artifacts(action.artifacts) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
	}

	RemoveAction::~RemoveAction() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	bool RemoveAction::addArtifact(Artifact* artifact) {
		if(!artifact)
			return false;
		if(!artifacts.insert(artifact).second)
			return false;
		artifact->ref();
		return true;
	}

	bool RemoveAction::removeArtifact(Artifact* artifact) {
		if(!artifacts.erase(artifact))
			return false;
		artifact->unref();
		return true;
	}

	void RemoveAction::clearArtifacts() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		artifacts.clear();
	}

	void RemoveAction::getArtifacts(ArtifactIterator& begin, ArtifactIterator& end) const {
		begin = artifacts.begin();
		end = artifacts.end();
	}

	void RemoveAction::perform(BuildContext&) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			if((*begin)->isPresent())
				(*begin)->remove();
	}

	void RemoveAction::wouldPerform(BuildContext&) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			if((*begin)->wouldBePresent())
				(*begin)->wouldRemove();
	}

	void RemoveAction::notifyUIWillPerform(BuildUI&) const {
		//TODO
	}

	void RemoveAction::notifyUIWouldPerform(BuildUI&) const {
		//TODO
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void RemoveAction::dumpAction(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "RemoveAction " << this << " {" << endln << shift;
		stream << indent << "artifacts = {" << endln << shift;
		ArtifactIterator abegin(artifacts.begin()), aend(artifacts.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}

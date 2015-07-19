#include "Artifact.hpp"
#include "RemoveAction.hpp"

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

}}

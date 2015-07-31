#include "Artifact.hpp"
#include "ArtifactGoal.hpp"

namespace redengine {
namespace build {

	ArtifactGoal::ArtifactGoal() {}

	ArtifactGoal::ArtifactGoal(const ArtifactGoal& goal) : Goal(goal), artifacts(goal.artifacts) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
	}

	ArtifactGoal::~ArtifactGoal() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	bool ArtifactGoal::addArtifact(Artifact& artifact) {
		if(!artifacts.append(&artifact))
			return false;
		artifact.ref();
		return true;
	}

	bool ArtifactGoal::removeArtifact(Artifact& artifact) {
		if(!artifacts.erase(&artifact))
			return false;
		artifact.unref();
		return true;
	}

	void ArtifactGoal::clearArtifacts() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		artifacts.clear();
	}

	void ArtifactGoal::getArtifacts(ArtifactIterator& begin, ArtifactIterator& end) const {
		begin = artifacts.begin();
		end = artifacts.end();
	}

	void ArtifactGoal::attain(BuildContext& context) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->require(context);
	}

	void ArtifactGoal::wouldAttain(BuildContext& context) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->wouldRequire(context);
	}

}}

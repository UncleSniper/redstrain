#include <redstrain/io/streamoperators.hpp>

#include "Artifact.hpp"
#include "RemoveGoal.hpp"

using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	RemoveGoal::RemoveGoal() {}

	RemoveGoal::RemoveGoal(const RemoveGoal& goal) : Goal(goal), artifacts(goal.artifacts) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
	}

	RemoveGoal::~RemoveGoal() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	bool RemoveGoal::addArtifact(Artifact& artifact) {
		if(!artifacts.append(&artifact))
			return false;
		artifact.ref();
		return true;
	}

	bool RemoveGoal::removeArtifact(Artifact& artifact) {
		if(!artifacts.erase(&artifact))
			return false;
		artifact.unref();
		return true;
	}

	void RemoveGoal::clearArtifacts() {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		artifacts.clear();
	}

	void RemoveGoal::getArtifacts(ArtifactIterator& begin, ArtifactIterator& end) const {
		begin = artifacts.begin();
		end = artifacts.end();
	}

	void RemoveGoal::attain(BuildContext&) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->remove();
	}

	void RemoveGoal::wouldAttain(BuildContext&) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin)
			(*begin)->wouldRemove();
	}

	void RemoveGoal::dumpGoal(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "RemoveGoal " << this << " {" << endln << shift;
		stream << indent << "artifacts = {" << endln << shift;
		ArtifactIterator abegin(artifacts.begin()), aend(artifacts.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
		stream << unshift << indent << '}' << endln;
	}

}}

#include <redstrain/io/streamoperators.hpp>

#include "BuildUI.hpp"
#include "Artifact.hpp"
#include "RemoveGoal.hpp"
#include "BuildContext.hpp"

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

	void RemoveGoal::attain(BuildContext& context) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin) {
			if((*begin)->isPresent()) {
				context.getUI().willPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
						"removing", (*begin)->getLabel(), ""), false);
				(*begin)->remove();
			}
		}
	}

	void RemoveGoal::wouldAttain(BuildContext& context) {
		ArtifactIterator begin(artifacts.begin()), end(artifacts.end());
		for(; begin != end; ++begin) {
			if((*begin)->wouldBePresent()) {
				context.getUI().wouldPerformAction(BuildUI::ActionDescriptor(getComponentType(), getComponentName(),
						"would remove", (*begin)->getLabel(), ""), false);
				(*begin)->wouldRemove();
			}
		}
	}

	void RemoveGoal::dumpGoal(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "RemoveGoal " << this << " {" << endln << shift;
		stream << indent << "artifacts = {" << endln << shift;
		ArtifactIterator abegin(artifacts.begin()), aend(artifacts.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
		dumpGoalAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}

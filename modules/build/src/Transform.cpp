#include <redstrain/io/streamoperators.hpp>

#include "Artifact.hpp"
#include "Transform.hpp"

using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	Transform::Transform() {}

	Transform::Transform(const Transform& transform)
			: ReferenceCounted(transform), prerequisites(transform.prerequisites) {
		PrerequisiteIterator pqbegin(prerequisites.begin()), pqend(prerequisites.end());
		for(; pqbegin != pqend; ++pqbegin)
			(*pqbegin)->ref();
	}

	Transform::~Transform() {
		PrerequisiteIterator pqbegin(prerequisites.begin()), pqend(prerequisites.end());
		for(; pqbegin != pqend; ++pqbegin)
			(*pqbegin)->unref();
	}

	bool Transform::addPrerequisite(Artifact& artifact) {
		if(!prerequisites.append(&artifact))
			return false;
		artifact.ref();
		return true;
	}

	bool Transform::removePrerequisite(Artifact& artifact) {
		if(!prerequisites.erase(&artifact))
			return false;
		artifact.unref();
		return true;
	}

	void Transform::clearPrerequisites() {
		PrerequisiteIterator pqbegin(prerequisites.begin()), pqend(prerequisites.end());
		for(; pqbegin != pqend; ++pqbegin)
			(*pqbegin)->unref();
		prerequisites.clear();
	}

	void Transform::getPrerequisites(PrerequisiteIterator& begin, PrerequisiteIterator& end) const {
		begin = prerequisites.begin();
		end = prerequisites.end();
	}

	void Transform::wouldPerform(BuildContext& context, Artifact& target) {
		target.wouldModify(context);
	}

	void Transform::dumpTransformAspects(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "prerequisites = {" << endln << shift;
		PrerequisiteIterator pqbegin(prerequisites.begin()), pqend(prerequisites.end());
		for(; pqbegin != pqend; ++pqbegin)
			(*pqbegin)->dumpArtifact(stream);
		stream << unshift << indent << '}' << endln;
	}

}}

#include "CopyTransform.hpp"

namespace redengine {
namespace build {

	CopyTransform::CopyTransform(FileArtifact& source) : OneToOneTransform<FileArtifact>(source) {}

	CopyTransform::CopyTransform(const CopyTransform& transform) : OneToOneTransform<FileArtifact>(transform) {}

	void CopyTransform::perform(BuildContext&, Artifact&) {
		//TODO
	}

	void CopyTransform::wouldPerform(BuildContext&, Artifact&) {
		//TODO
	}

}}

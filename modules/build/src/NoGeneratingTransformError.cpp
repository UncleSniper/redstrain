#include "Artifact.hpp"
#include "NoGeneratingTransformError.hpp"

namespace redengine {
namespace build {

	NoGeneratingTransformError::NoGeneratingTransformError(Artifact& artifact) : artifact(&artifact) {
		artifact.ref();
	}

	NoGeneratingTransformError::NoGeneratingTransformError(const NoGeneratingTransformError& error)
			: BuildError(error), artifact(error.artifact) {
		artifact->ref();
	}

	NoGeneratingTransformError::~NoGeneratingTransformError() {
		artifact->unref();
	}

	REDSTRAIN_DEFINE_ERROR(NoGeneratingTransformError) {
		out << "No generating transform for '" << artifact->getLabel() << "'";
	}

}}

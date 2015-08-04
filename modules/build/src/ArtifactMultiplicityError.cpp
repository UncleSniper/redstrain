#include "Artifact.hpp"
#include "ArtifactMultiplicityError.hpp"

using std::string;

namespace redengine {
namespace build {

	ArtifactMultiplicityError::ArtifactMultiplicityError(Artifact& artifact, const string& message)
			: artifact(&artifact), message(message) {
		artifact.ref();
	}

	ArtifactMultiplicityError::ArtifactMultiplicityError(const ArtifactMultiplicityError& error)
			: BuildError(error), artifact(error.artifact), message(error.message) {
		artifact->ref();
	}

	ArtifactMultiplicityError::~ArtifactMultiplicityError() {
		artifact->unref();
	}

	REDSTRAIN_DEFINE_ERROR(ArtifactMultiplicityError) {
		out << message;
		string label(artifact->getLabel());
		if(!label.empty())
			out << ": " << label;
	}

}}

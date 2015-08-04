#ifndef REDSTRAIN_MOD_BUILD_ARTIFACTMULTIPLICITYERROR_HPP
#define REDSTRAIN_MOD_BUILD_ARTIFACTMULTIPLICITYERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class ArtifactMultiplicityError : public BuildError {

	  private:
		Artifact *const artifact;
		const std::string message;

	  public:
		ArtifactMultiplicityError(Artifact&, const std::string&);
		ArtifactMultiplicityError(const ArtifactMultiplicityError&);
		virtual ~ArtifactMultiplicityError();

		inline const Artifact& getArtifact() const {
			return *artifact;
		}

		inline const std::string& getRawMessage() const {
			return message;
		}

		REDSTRAIN_DECLARE_ERROR(ArtifactMultiplicityError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ARTIFACTMULTIPLICITYERROR_HPP */

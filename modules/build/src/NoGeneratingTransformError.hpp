#ifndef REDSTRAIN_MOD_BUILD_NOGENERATINGTRANSFORMERROR_HPP
#define REDSTRAIN_MOD_BUILD_NOGENERATINGTRANSFORMERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class Artifact;

	class REDSTRAIN_BUILD_API NoGeneratingTransformError : public BuildError {

	  private:
		Artifact *const artifact;

	  public:
		NoGeneratingTransformError(Artifact&);
		NoGeneratingTransformError(const NoGeneratingTransformError&);
		virtual ~NoGeneratingTransformError();

		inline const Artifact& getArtifact() const {
			return *artifact;
		}

		REDSTRAIN_DECLARE_ERROR(NoGeneratingTransformError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_NOGENERATINGTRANSFORMERROR_HPP */

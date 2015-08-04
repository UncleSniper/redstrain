#ifndef REDSTRAIN_MOD_BUILD_ARTIFACTSTAGE_HPP
#define REDSTRAIN_MOD_BUILD_ARTIFACTSTAGE_HPP

#include <string>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class BuildUI;
	class Artifact;

	class REDSTRAIN_BUILD_API ArtifactStage : public util::ReferenceCounted {

	  private:
		std::string directory, label;

	  public:
		ArtifactStage(const std::string&);
		ArtifactStage(const std::string&, const std::string&);
		ArtifactStage(const ArtifactStage&);

		inline const std::string& getDirectory() const {
			return directory;
		}

		void setDirectory(const std::string&);

		inline const std::string& getLabel() const {
			return label;
		}

		void setLabel(const std::string&);

		void stage(Artifact&, const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_ARTIFACTSTAGE_HPP */

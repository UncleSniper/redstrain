#ifndef REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP

#include <string>

#include "Artifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API FileArtifact : public Artifact {

	  private:
		const std::string pathname;

	  public:
		FileArtifact(const std::string&);
		FileArtifact(const FileArtifact&);

		inline const std::string getPathname() const {
			return pathname;
		}

		virtual bool isPresent();
		virtual time_t getModificationTimestamp();
		virtual void remove();

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP */

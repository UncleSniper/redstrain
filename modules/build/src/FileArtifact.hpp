#ifndef REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP

#include <string>

#include "Artifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API FileArtifact : public Artifact {

	  private:
		const std::string directory, basename;

	  public:
		FileArtifact(const std::string&);
		FileArtifact(const std::string&, const std::string&);
		FileArtifact(const FileArtifact&);

		inline const std::string& getDirectory() const {
			return directory;
		}

		inline const std::string& getBasename() const {
			return basename;
		}

		std::string getPathname() const;

		virtual bool isPresent();
		virtual time_t getModificationTimestamp();
		virtual void remove();

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpArtifact(io::DefaultConfiguredOutputStream<char>::Stream&) const;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP */

#ifndef REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP
#define REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP

#include "Artifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API FileArtifact : public Artifact {

	  private:
		const std::string directory, basename;

	  private:
		void initializePredictedState();

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
		virtual std::string getHumanReadableReference(bool) const;

		virtual void dumpArtifact(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FILEARTIFACT_HPP */

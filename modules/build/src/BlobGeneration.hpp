#ifndef REDSTRAIN_MOD_BUILD_BLOBGENERATION_HPP
#define REDSTRAIN_MOD_BUILD_BLOBGENERATION_HPP

#include "Generation.hpp"
#include "BlobLanguage.hpp"
#include "FileArtifact.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API BlobGeneration : public Generation<FileArtifact> {

	  private:
		BlobLanguage::BlobConfiguration& configuration;
		bool header;

	  private:
		static FileArtifact* getSoleSource(const std::list<FileArtifact*>&);

	  public:
		BlobGeneration(BlobLanguage::BlobConfiguration&, bool);
		BlobGeneration(const BlobGeneration&);

		inline BlobLanguage::BlobConfiguration& getBlobConfiguration() {
			return configuration;
		}

		inline const BlobLanguage::BlobConfiguration& getBlobConfiguration() const {
			return configuration;
		}

		inline bool isHeader() const {
			return header;
		}

		virtual void generate(const std::list<FileArtifact*>&, FileArtifact*, BuildContext&);
		virtual void notifyUIWillGenerate(BuildUI&, const Action&, const std::list<FileArtifact*>&, FileArtifact*);
		virtual void notifyUIWouldGenerate(BuildUI&, const Action&, const std::list<FileArtifact*>&, FileArtifact*);

		virtual void dumpGeneration(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BLOBGENERATION_HPP */

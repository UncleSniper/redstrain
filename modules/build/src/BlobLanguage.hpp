#ifndef REDSTRAIN_MOD_BUILD_BLOBLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BLOBLANGUAGE_HPP

#include <redstrain/io/CPPArrayOutputStream.hpp>

#include "FileConversionLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API BlobLanguage : public FileConversionLanguage {

	  public:
		class REDSTRAIN_BUILD_API BlobConfiguration {

		  public:
			BlobConfiguration();
			BlobConfiguration(const BlobConfiguration&);
			virtual ~BlobConfiguration();

			virtual void applyConfiguration(io::CPPArrayOutputStream&) = 0;

		};

		class REDSTRAIN_BUILD_API EmptyBlobConfiguration : public BlobConfiguration {

		  public:
			EmptyBlobConfiguration();
			EmptyBlobConfiguration(const EmptyBlobConfiguration&);

			virtual void applyConfiguration(io::CPPArrayOutputStream&);

		  public:
			static EmptyBlobConfiguration instance;

		};

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&, const Component&);
		virtual GenerationAction<FileArtifact>* newGenerationAction(FileArtifact*, const Flavor&, const Flavor&,
				const Component&, BuildContext&);
		virtual BlobConfiguration& getBlobConfiguration(const Flavor&, const Component&);

	  public:
		BlobLanguage();
		BlobLanguage(const BlobLanguage&);

		virtual ArtifactType classifyFile(const std::string&);
		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BLOBLANGUAGE_HPP */

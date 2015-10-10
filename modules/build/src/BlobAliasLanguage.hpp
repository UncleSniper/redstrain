#ifndef REDSTRAIN_MOD_BUILD_BLOBALIASLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BLOBALIASLANGUAGE_HPP

#include <redstrain/vfs/BlobLinkerDefinitionGenerator.hpp>

#include "FileConversionLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API BlobAliasLanguage : public FileConversionLanguage {

	  public:
		class REDSTRAIN_BUILD_API AliasConfiguration : public util::ReferenceCounted {

		  public:
			AliasConfiguration();
			AliasConfiguration(const AliasConfiguration&);
			virtual ~AliasConfiguration();

			virtual void applyConfiguration(vfs::BlobLinkerDefinitionGenerator&) = 0;

		};

		class REDSTRAIN_BUILD_API GenericAliasConfiguration : public AliasConfiguration {

		  private:
			std::string pathPrefix, fileSuffix;

		  public:
			GenericAliasConfiguration();
			GenericAliasConfiguration(const std::string&, const std::string&);
			GenericAliasConfiguration(const GenericAliasConfiguration&);
			virtual ~GenericAliasConfiguration();

			inline const std::string& getPathPrefix() const {
				return pathPrefix;
			}

			void setPathPrefix(const std::string&);

			inline const std::string getFileSuffix() const {
				return fileSuffix;
			}

			void setFileSuffix(const std::string&);

			virtual void applyConfiguration(vfs::BlobLinkerDefinitionGenerator&);

		};

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&, Component&);
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);
		virtual AliasConfiguration* getAliasConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);

	  public:
		BlobAliasLanguage();
		BlobAliasLanguage(const BlobAliasLanguage&);

		virtual ArtifactType classifyFile(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BLOBALIASLANGUAGE_HPP */

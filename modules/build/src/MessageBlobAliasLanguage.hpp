#ifndef REDSTRAIN_MOD_BUILD_MESSAGEBLOBALIASLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_MESSAGEBLOBALIASLANGUAGE_HPP

#include "FileConversionLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API MessageBlobAliasLanguage : public FileConversionLanguage {

	  public:
		class REDSTRAIN_BUILD_API AliasConfiguration : public util::ReferenceCounted {

		  private:
			std::string mappingSymbol, blobNSPrefix;

		  public:
			AliasConfiguration(const std::string&, const std::string&);
			AliasConfiguration(const AliasConfiguration&);

			inline const std::string getMappingSymbol() const {
				return mappingSymbol;
			}

			void setMappingSymbol(const std::string&);

			inline const std::string getBlobNamespacePrefix() const {
				return blobNSPrefix;
			}

			void setBlobNamespacePrefix(const std::string&);

		};

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&, Component&);
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&, BuildContext&);
		virtual AliasConfiguration& getAliasConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&) = 0;

	  public:
		MessageBlobAliasLanguage();
		MessageBlobAliasLanguage(const MessageBlobAliasLanguage&);

		virtual ArtifactType classifyFile(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_MESSAGEBLOBALIASLANGUAGE_HPP */

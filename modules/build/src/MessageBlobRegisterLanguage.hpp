#ifndef REDSTRAIN_MOD_BUILD_MESSAGEBLOBREGISTERLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_MESSAGEBLOBREGISTERLANGUAGE_HPP

#include "FileConversionLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API MessageBlobRegisterLanguage : public FileConversionLanguage {

	  public:
		class REDSTRAIN_BUILD_API RegistrarConfiguration : public util::ReferenceCounted {

		  private:
			std::string mappingSymbol, blobSymbol, language, country;

		  public:
			RegistrarConfiguration(const std::string&, const std::string&, const std::string&, const std::string&);
			RegistrarConfiguration(const RegistrarConfiguration&);
			virtual ~RegistrarConfiguration();

			inline const std::string& getMappingSymbol() const {
				return mappingSymbol;
			}

			void setMappingSymbol(const std::string&);

			inline const std::string& getBlobSymbol() const {
				return blobSymbol;
			}

			void setBlobSymbol(const std::string&);

			inline const std::string& getLanguage() const {
				return language;
			}

			void setLanguage(const std::string&);

			inline const std::string& getCountry() const {
				return country;
			}

			void setCountry(const std::string&);

		};

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&, Component&);
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&, BuildContext&);
		virtual RegistrarConfiguration& getRegistrarConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&) = 0;

	  public:
		MessageBlobRegisterLanguage();
		MessageBlobRegisterLanguage(const MessageBlobRegisterLanguage&);

		virtual ArtifactType classifyFile(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_MESSAGEBLOBREGISTERLANGUAGE_HPP */

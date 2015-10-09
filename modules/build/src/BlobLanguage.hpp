#ifndef REDSTRAIN_MOD_BUILD_BLOBLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BLOBLANGUAGE_HPP

#include <redstrain/io/CPPArrayOutputStream.hpp>

#include "FileConversionLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API BlobLanguage : public FileConversionLanguage {

	  public:
		class REDSTRAIN_BUILD_API BlobConfiguration : public util::ReferenceCounted {

		  public:
			BlobConfiguration();
			BlobConfiguration(const BlobConfiguration&);
			virtual ~BlobConfiguration();

			virtual void applyConfiguration(io::CPPArrayOutputStream&) = 0;

		};

		class REDSTRAIN_BUILD_API GenericBlobConfiguration : public BlobConfiguration {

		  private:
			std::string variable, exportMacro, blobPath, extraInclude, guardMacro;

		  public:
			GenericBlobConfiguration();
			GenericBlobConfiguration(const std::string&, const std::string&,
					const std::string&, const std::string&, const std::string&);
			GenericBlobConfiguration(const GenericBlobConfiguration&);

			inline const std::string& getVariableName() const {
				return variable;
			}

			void setVariableName(const std::string&);

			inline const std::string& getExportMacro() const {
				return exportMacro;
			}

			void setExportMacro(const std::string&);

			inline const std::string& getBlobPath() const {
				return blobPath;
			}

			void setBlobPath(const std::string&);

			inline const std::string& getExtraInclude() const {
				return extraInclude;
			}

			void setExtraInclude(const std::string&);

			inline const std::string& getGuardMacro() const {
				return guardMacro;
			}

			void setGuardMacro(const std::string&);

			virtual void applyConfiguration(io::CPPArrayOutputStream&);

		};

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&, Component&);
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);
		virtual BlobConfiguration* getBlobConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);

	  public:
		BlobLanguage();
		BlobLanguage(const BlobLanguage&);

		virtual ArtifactType classifyFile(const std::string&);
		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BLOBLANGUAGE_HPP */

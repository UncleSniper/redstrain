#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKECODETABLE16REGISTERLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKECODETABLE16REGISTERLANGUAGE_HPP

#include "../CodeTable16RegisterLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeCodeTable16RegisterLanguage : public CodeTable16RegisterLanguage {

	  public:
		class REDSTRAIN_BUILD_API XakeRegistrarConfiguration : public RegistrarConfiguration {

		  private:
			text::BlobCodeTable16Registrar::GeneratorWriter::DefaultBlobSymbolMapper blobSymbolMapper;

		  public:
			XakeRegistrarConfiguration(const std::string&);
			XakeRegistrarConfiguration(const XakeRegistrarConfiguration&);
			virtual ~XakeRegistrarConfiguration();

			virtual void applyConfiguration(text::BlobCodeTable16Registrar::GeneratorReader&);
			virtual void applyConfiguration(text::BlobCodeTable16Registrar::GeneratorWriter&);

		};

	  private:
		const XakeProject& project;

	  protected:
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);
		virtual RegistrarConfiguration* getRegistrarConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);

	  public:
		XakeCodeTable16RegisterLanguage(const XakeProject&);
		XakeCodeTable16RegisterLanguage(const XakeCodeTable16RegisterLanguage&);

		inline const XakeProject& getProject() const {
			return project;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKECODETABLE16REGISTERLANGUAGE_HPP */

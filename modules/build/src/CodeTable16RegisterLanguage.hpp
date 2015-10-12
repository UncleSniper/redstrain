#ifndef REDSTRAIN_MOD_BUILD_CODETABLE16REGISTERLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_CODETABLE16REGISTERLANGUAGE_HPP

#include <redstrain/text/BlobCodeTable16Registrar.hpp>

#include "FileConversionLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CodeTable16RegisterLanguage : public FileConversionLanguage {

	  public:
		class REDSTRAIN_BUILD_API RegistrarConfiguration : public util::ReferenceCounted {

		  public:
			RegistrarConfiguration();
			RegistrarConfiguration(const RegistrarConfiguration&);
			virtual ~RegistrarConfiguration();

			virtual void applyConfiguration(text::BlobCodeTable16Registrar::GeneratorReader&) = 0;
			virtual void applyConfiguration(text::BlobCodeTable16Registrar::GeneratorWriter&) = 0;

		};

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&, Component&);
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);
		virtual RegistrarConfiguration* getRegistrarConfiguration(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);

	  public:
		CodeTable16RegisterLanguage();
		CodeTable16RegisterLanguage(const CodeTable16RegisterLanguage&);

		virtual ArtifactType classifyFile(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CODETABLE16REGISTERLANGUAGE_HPP */

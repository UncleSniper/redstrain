#ifndef REDSTRAIN_MOD_BUILD_MESSAGES16DEFINITIONLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_MESSAGES16DEFINITIONLANGUAGE_HPP

#include "FileConversionLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API Messages16DefinitionLanguage : public FileConversionLanguage {

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&, Component&);
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&, BuildContext&);

	  public:
		Messages16DefinitionLanguage();
		Messages16DefinitionLanguage(const Messages16DefinitionLanguage&);

		virtual ArtifactType classifyFile(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_MESSAGES16DEFINITIONLANGUAGE_HPP */

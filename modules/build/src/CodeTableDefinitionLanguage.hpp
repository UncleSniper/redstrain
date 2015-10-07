#ifndef REDSTRAIN_MOD_BUILD_CODETABLEDEFINITIONLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_CODETABLEDEFINITIONLANGUAGE_HPP

#include "FileConversionLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CodeTableDefinitionLanguage : public FileConversionLanguage {

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&, Component&);
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&);

	  public:
		CodeTableDefinitionLanguage();
		CodeTableDefinitionLanguage(const CodeTableDefinitionLanguage&);

		virtual ArtifactType classifyFile(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CODETABLEDEFINITIONLANGUAGE_HPP */

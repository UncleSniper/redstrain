#ifndef REDSTRAIN_MOD_BUILD_FILECONVERSIONLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_FILECONVERSIONLANGUAGE_HPP

#include "Language.hpp"
#include "GenerationAction.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API FileConversionLanguage : public Language {

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&,
				const Component&) = 0;
		virtual GenerationAction<FileArtifact>* newGenerationAction(FileArtifact*, FileArtifact*, const Flavor&,
				const Flavor&, const Component&, BuildContext&) = 0;

	  public:
		FileConversionLanguage(const std::string&);
		FileConversionLanguage(const FileConversionLanguage&);

		virtual bool acceptsSource(const Flavor&, const Flavor&);
		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&);
		virtual bool isOneToOne(const Flavor&);
		virtual Component::GenerationHolder* getGenerationTrigger(BuildContext&, const std::string&,
				const std::string&, const Flavor&, const std::string&, const Flavor&, Component&,
				Component::BuildArtifactMapper&);
		virtual Flavor getGeneratedSourceFlavor(const Flavor&, const Flavor&, const std::string&);
		virtual Flavor getGeneratedHeaderFlavor(const Flavor&, const Flavor&, const std::string&);
		virtual Component::GenerationHolder* getHeaderExposeTrigger(BuildContext&, const std::string&,
				const std::string&, const Flavor&, const std::string&, const Flavor&, Component&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FILECONVERSIONLANGUAGE_HPP */

#ifndef REDSTRAIN_MOD_BUILD_FILECONVERSIONLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_FILECONVERSIONLANGUAGE_HPP

#include "Language.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API FileConversionLanguage : public Language {

	  protected:
		virtual std::string getTargetBasename(const std::string&, const Flavor&, const Flavor&, Component&) = 0;
		virtual Transform* getConversionTransform(FileArtifact&, const Flavor&,
				FileArtifact&, const Flavor&, const Flavor&, Component&, BuildContext&) = 0;
		virtual Flavor getTargetFlavor(const Flavor&, const Flavor&);
		virtual bool isTargetFinal(FileArtifact&, const Flavor&, const Flavor&,
				FileArtifact&, const Flavor&, Component&);

	  public:
		FileConversionLanguage(const std::string&);
		FileConversionLanguage(const FileConversionLanguage&);

		virtual bool acceptsSource(const Flavor&, const Flavor&);
		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&);
		virtual bool isOneToOne(const Flavor&);
		virtual FileArtifact* getSourceTransform(BuildContext&, const std::string&, FileArtifact&,
				const Flavor&, const std::string&, const Flavor&, Component&, BuildArtifactMapper&,
				ManyToOneTransform<FileArtifact>*&, Flavor&, bool&);
		virtual FileArtifact* getHeaderExposeTransform(BuildContext&, const std::string&, FileArtifact&,
				const Flavor&, const std::string&, Component&, BuildArtifactMapper&, Flavor&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_FILECONVERSIONLANGUAGE_HPP */

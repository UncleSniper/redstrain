#ifndef REDSTRAIN_MOD_BUILD_OBJECTFILELANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_OBJECTFILELANGUAGE_HPP

#include "Language.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ObjectFileLanguage : public Language {

	  public:
		ObjectFileLanguage(const std::string&);
		ObjectFileLanguage(const ObjectFileLanguage&);

		virtual bool isOneToOne(const Flavor&);
		virtual FileArtifact* getHeaderExposeTransform(BuildContext&, const std::string&, FileArtifact&,
				const Flavor&, const std::string&, Component&, BuildArtifactMapper&, Flavor&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_OBJECTFILELANGUAGE_HPP */

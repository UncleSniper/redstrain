#ifndef REDSTRAIN_MOD_BUILD_LANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_LANGUAGE_HPP

#include <string>
#include <redstrain/util/Appender.hpp>

#include "Component.hpp"

namespace redengine {
namespace build {

	class Flavor;

	class REDSTRAIN_BUILD_API Language {

	  public:
		enum REDSTRAIN_BUILD_API ArtifactType {
			AT_NONE,
			AT_SOURCE,
			AT_HEADER
		};

	  private:
		const std::string name;

	  public:
		Language(const std::string&);
		Language(const Language&);
		virtual ~Language();

		inline const std::string getName() const {
			return name;
		}

		virtual ArtifactType classifyFile(const std::string&) = 0;
		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&) = 0;
		virtual std::string getArtifactNameForSource(Component::Type, const Flavor&, const std::string&) = 0;
		virtual std::string getPrimaryArtifactName(Component::Type, const Flavor&, const std::string&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LANGUAGE_HPP */

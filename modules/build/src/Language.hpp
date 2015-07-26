#ifndef REDSTRAIN_MOD_BUILD_LANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_LANGUAGE_HPP

#include <string>
#include <redstrain/util/Appender.hpp>

#include "Flavor.hpp"
#include "Component.hpp"

namespace redengine {
namespace build {

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

		virtual Flavor getShippedSourceFlavor();
		virtual Flavor getShippedHeaderFlavor();
		virtual ArtifactType classifyFile(const std::string&) = 0;
		virtual bool acceptsSource(const Flavor&, const Flavor&) = 0;
		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&) = 0;
		virtual bool isOneToOne(const Flavor&) = 0;
		virtual Component::GenerationHolder* getGenerationTrigger(BuildContext&, const std::string&,
				const std::string&, const Flavor&, const std::string&, const Flavor&, Component&,
				Component::BuildArtifactMapper&) = 0;
		virtual Flavor getGeneratedSourceFlavor(const Flavor&, const Flavor&, const std::string&) = 0;
		virtual Flavor getGeneratedHeaderFlavor(const Flavor&, const Flavor&, const std::string&) = 0;
		virtual Flavor getHeaderExposeTransformFlavor();
		virtual Flavor getCleanFlavor();
		virtual Component::GenerationHolder* getHeaderExposeTrigger(BuildContext&, const std::string&,
				const std::string&, const Flavor&, const std::string&, const Flavor&) = 0;

		static ArtifactType classifyFileBySuffix(const std::string&, const char *const*, const char *const*);
		static void sinkSupportedFlavors(const Flavor *const*, util::Appender<Flavor>&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_LANGUAGE_HPP */

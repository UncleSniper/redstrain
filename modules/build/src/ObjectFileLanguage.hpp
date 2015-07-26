#ifndef REDSTRAIN_MOD_BUILD_OBJECTFILELANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_OBJECTFILELANGUAGE_HPP

#include "Language.hpp"

namespace redengine {
namespace build {

	class Linker;
	class LinkerConfiguration;
	class LinkGenerationAction;

	class REDSTRAIN_BUILD_API ObjectFileLanguage : public Language {

	  public:
		class REDSTRAIN_BUILD_API LinkGenerationHolder : public Component::GenerationHolder {

		  private:
			GenerationTrigger* trigger;
			LinkGenerationAction* action;
			Component::PreciousArtifact* preciousArtifact;

		  public:
			LinkGenerationHolder(GenerationTrigger*, LinkGenerationAction*, Component::PreciousArtifact*);
			LinkGenerationHolder(const LinkGenerationHolder&);
			virtual ~LinkGenerationHolder();

			inline LinkGenerationAction* getLinkGenerationAction() const {
				return action;
			}

			virtual Trigger* getTrigger();
			virtual void addSource(FileArtifact*);
			virtual void getTargets(GenerationTrigger::ArtifactIterator&, GenerationTrigger::ArtifactIterator&);
			virtual Component::PreciousArtifact* getPreciousArtifact();

		};

	  private:
		Linker& linker;

	  protected:
		virtual LinkerConfiguration& getLinkerConfiguration(const Flavor&, const Component&);

	  public:
		ObjectFileLanguage(const std::string&, Linker&);
		ObjectFileLanguage(const ObjectFileLanguage&);

		inline Linker& getLinker() {
			return linker;
		}

		inline const Linker& getLinker() const {
			return linker;
		}

		virtual ArtifactType classifyFile(const std::string&);
		virtual bool acceptsSource(const Flavor&, const Flavor&);
		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&);
		virtual bool isOneToOne(const Flavor&);
		virtual Component::GenerationHolder* getGenerationTrigger(BuildContext&, const std::string&,
				const std::string&, const Flavor&, const std::string&, const Flavor&, Component&,
				Component::BuildArtifactMapper&);
		virtual Flavor getGeneratedSourceFlavor(const Flavor&, const Flavor&, const std::string&);
		virtual Flavor getGeneratedHeaderFlavor(const Flavor&, const Flavor&, const std::string&);
		virtual Component::GenerationHolder* getHeaderExposeTrigger(BuildContext&, const std::string&,
				const std::string&, const Flavor&, const std::string&, const Flavor&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_OBJECTFILELANGUAGE_HPP */

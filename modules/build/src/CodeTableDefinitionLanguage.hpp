#ifndef REDSTRAIN_MOD_BUILD_CODETABLEDEFINITIONLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_CODETABLEDEFINITIONLANGUAGE_HPP

#include "Language.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CodeTableDefinitionLanguage : public Language {

	  public:
		class REDSTRAIN_BUILD_API CodeTableCompileGenerationHolder : public Component::GenerationHolder {

		  private:
			GenerationTrigger* trigger;

		  public:
			CodeTableCompileGenerationHolder(GenerationTrigger*);
			virtual ~CodeTableCompileGenerationHolder();

			virtual Trigger* getTrigger();
			virtual void addSource(FileArtifact*);
			virtual void addTriggerSource(FileArtifact*);
			virtual void getTargets(GenerationTrigger::ArtifactIterator&, GenerationTrigger::ArtifactIterator&);

		};

	  public:
		CodeTableDefinitionLanguage();
		CodeTableDefinitionLanguage(const CodeTableDefinitionLanguage&);

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
				const std::string&, const Flavor&, const std::string&, const Flavor&, Component&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CODETABLEDEFINITIONLANGUAGE_HPP */

#ifndef REDSTRAIN_MOD_BUILD_COMPILEDLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_COMPILEDLANGUAGE_HPP

#include "Language.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CompiledLanguage : public Language {

/*
	  public:
		class REDSTRAIN_BUILD_API CompileGenerationHolder : public Component::GenerationHolder {

		  public:
			CompileGenerationHolder();
			CompileGenerationHolder(const CompileGenerationHolder&);

			virtual Trigger* getTrigger();
			virtual void addSource(FileArtifact*);
			virtual void getTargets(GenerationTrigger::ArtifactIterator&, GenerationTrigger::ArtifactIterator&);

		};
*/

	  public:
		CompiledLanguage(const std::string&);
		CompiledLanguage(const CompiledLanguage&);

		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&);
		virtual bool isOneToOne(const Flavor&);
		virtual Component::GenerationHolder* getGenerationTrigger(const std::string&, const std::string&,
				const Flavor&, const std::string&, const Flavor&, Component&);
		virtual Flavor getGeneratedSourceFlavor(const Flavor&, const Flavor&, const std::string&);
		virtual Flavor getGeneratedHeaderFlavor(const Flavor&, const Flavor&, const std::string&);
		virtual Component::GenerationHolder* getHeaderExposeTrigger(const std::string&, const std::string&,
				const Flavor&, const std::string&, const Flavor&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILEDLANGUAGE_HPP */

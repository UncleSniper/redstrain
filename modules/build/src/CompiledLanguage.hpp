#ifndef REDSTRAIN_MOD_BUILD_COMPILEDLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_COMPILEDLANGUAGE_HPP

#include "Language.hpp"

namespace redengine {
namespace build {

	class Compiler;
	class CompilerConfiguration;
	class CompileGenerationAction;

	class REDSTRAIN_BUILD_API CompiledLanguage : public Language {

	  public:
		class REDSTRAIN_BUILD_API CompileGenerationHolder : public Component::GenerationHolder {

		  private:
			GenerationTrigger* trigger;
			CompileGenerationAction* action;

		  public:
			CompileGenerationHolder(GenerationTrigger*, CompileGenerationAction*);
			CompileGenerationHolder(const CompileGenerationHolder&);
			virtual ~CompileGenerationHolder();

			inline CompileGenerationAction* getCompileGenerationAction() const {
				return action;
			}

			virtual Trigger* getTrigger();
			virtual void addSource(FileArtifact*);
			virtual void getTargets(GenerationTrigger::ArtifactIterator&, GenerationTrigger::ArtifactIterator&);

		};

		class REDSTRAIN_BUILD_API ExposeGenerationHolder : public Component::GenerationHolder {

		  private:
			GenerationTrigger* trigger;

		  public:
			ExposeGenerationHolder(GenerationTrigger*);
			ExposeGenerationHolder(const ExposeGenerationHolder&);
			virtual ~ExposeGenerationHolder();

			virtual Trigger* getTrigger();
			virtual void addSource(FileArtifact*);
			virtual void getTargets(GenerationTrigger::ArtifactIterator&, GenerationTrigger::ArtifactIterator&);

		};

	  private:
		Compiler& compiler;

	  protected:
		virtual CompilerConfiguration& getCompilerConfiguration(const Flavor&, const Component&);

	  public:
		CompiledLanguage(const std::string&, Compiler&);
		CompiledLanguage(const CompiledLanguage&);

		inline Compiler& getCompiler() {
			return compiler;
		}

		inline const Compiler& getCompiler() const {
			return compiler;
		}

		virtual void getSupportedFlavors(Component::Type, util::Appender<Flavor>&);
		virtual bool isOneToOne(const Flavor&);
		virtual Component::GenerationHolder* getGenerationTrigger(BuildContext&, const std::string&,
				const std::string&, const Flavor&, const std::string&, const Flavor&, Component&);
		virtual Flavor getGeneratedSourceFlavor(const Flavor&, const Flavor&, const std::string&);
		virtual Flavor getGeneratedHeaderFlavor(const Flavor&, const Flavor&, const std::string&);
		virtual Component::GenerationHolder* getHeaderExposeTrigger(BuildContext&, const std::string&,
				const std::string&, const Flavor&, const std::string&, const Flavor&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILEDLANGUAGE_HPP */

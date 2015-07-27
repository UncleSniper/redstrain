#ifndef REDSTRAIN_MOD_BUILD_CPPLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_CPPLANGUAGE_HPP

#include "CompiledLanguage.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API CPPLanguage : public CompiledLanguage {

	  public:
		class REDSTRAIN_BUILD_API CPPCompileGenerationHolder : public CompileGenerationHolder {

		  private:
			CPPLanguage& language;
			FileArtifact* source;

		  public:
			CPPCompileGenerationHolder(CPPLanguage&, GenerationTrigger*, CompileGenerationAction*, FileArtifact*);
			CPPCompileGenerationHolder(const CPPCompileGenerationHolder&);
			virtual ~CPPCompileGenerationHolder();

			inline CPPLanguage& getLanguage() {
				return language;
			}

			inline const CPPLanguage& getLanguage() const {
				return language;
			}

			inline FileArtifact* getSource() const {
				return source;
			}

			virtual bool evokesDependencySources();
			virtual void addDependencySources(const Component&);

		};

	  protected:
		virtual Component::GenerationHolder* newCompileGenerationHolder(GenerationTrigger*,
				CompileGenerationAction*);

	  public:
		CPPLanguage(Compiler&);
		CPPLanguage(const CPPLanguage&);

		virtual ArtifactType classifyFile(const std::string&);
		virtual void getReferencedHeaders(const std::string&, util::Appender<ReferencedHeader>&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_CPPLANGUAGE_HPP */

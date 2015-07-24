#ifndef REDSTRAIN_MOD_BUILD_COMPILEGENERATION_HPP
#define REDSTRAIN_MOD_BUILD_COMPILEGENERATION_HPP

#include "Compiler.hpp"
#include "Generation.hpp"
#include "FileArtifact.hpp"

namespace redengine {
namespace build {

	class CompilerConfiguration;

	class REDSTRAIN_BUILD_API CompileGeneration : public Generation<FileArtifact> {

	  private:
		Compiler& compiler;
		Compilation::CompileMode mode;
		CompilerConfiguration& configuration;

	  private:
		static FileArtifact* getSoleSource(const std::list<FileArtifact*>&);

	  public:
		CompileGeneration(Compiler&, Compilation::CompileMode, CompilerConfiguration&);
		CompileGeneration(const CompileGeneration&);

		inline Compiler& getCompiler() {
			return compiler;
		}

		inline const Compiler& getCompiler() const {
			return compiler;
		}

		inline Compilation::CompileMode getCompileMode() const {
			return mode;
		}

		inline void setCompileMode(Compilation::CompileMode mode) {
			this->mode = mode;
		}

		inline CompilerConfiguration& getCompilerConfiguration() {
			return configuration;
		}

		inline const CompilerConfiguration& getCompilerConfiguration() const {
			return configuration;
		}

		virtual void generate(const std::list<FileArtifact*>&, FileArtifact*, BuildContext&);
		virtual void notifyUIWillGenerate(BuildUI&, const Action&, const std::list<FileArtifact*>&, FileArtifact*);
		virtual void notifyUIWouldGenerate(BuildUI&, const Action&, const std::list<FileArtifact*>&, FileArtifact*);

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpGeneration(io::DefaultConfiguredOutputStream<char>::Stream&) const;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILEGENERATION_HPP */

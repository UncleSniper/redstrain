#ifndef REDSTRAIN_MOD_BUILD_COMPILETRANSFORM_HPP
#define REDSTRAIN_MOD_BUILD_COMPILETRANSFORM_HPP

#include "Compilation.hpp"
#include "FileArtifact.hpp"
#include "OneToOneTransform.hpp"

namespace redengine {
namespace build {

	class Compiler;
	class CompilerConfiguration;

	class REDSTRAIN_BUILD_API CompileTransform : public OneToOneTransform<FileArtifact> {

	  private:
		Compiler& compiler;
		Compilation::CompileMode mode;
		CompilerConfiguration& configuration;

	  public:
		CompileTransform(Compiler&, Compilation::CompileMode, CompilerConfiguration&, FileArtifact&);
		CompileTransform(const CompileTransform&);

		inline Compiler& getCompiler() {
			return compiler;
		}

		inline const Compiler& getCompiler() const {
			return compiler;
		}

		inline Compilation::CompileMode getCompileMode() const {
			return mode;
		}

		inline CompilerConfiguration& getCompilerConfiguration() {
			return configuration;
		}

		inline const CompilerConfiguration& getCompilerConfiguration() const {
			return configuration;
		}

		virtual void perform(BuildContext&, Artifact&);
		virtual void wouldPerform(BuildContext&, Artifact&);
		virtual bool onlyModifiesTarget() const;

		virtual void dumpTransform(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILETRANSFORM_HPP */

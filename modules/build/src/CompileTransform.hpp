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

		virtual void perform(BuildContext&, Artifact&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPILETRANSFORM_HPP */

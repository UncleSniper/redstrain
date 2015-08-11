#include "CompileTransform.hpp"

namespace redengine {
namespace build {

	CompileTransform::CompileTransform(Compiler& compiler, Compilation::CompileMode mode,
			CompilerConfiguration& configuration, FileArtifact& source) : OneToOneTransform<FileArtifact>(source),
			compiler(compiler), mode(mode), configuration(configuration) {}

	CompileTransform::CompileTransform(const CompileTransform& transform)
			: OneToOneTransform<FileArtifact>(transform), compiler(transform.compiler), mode(transform.mode),
			configuration(transform.configuration) {}

	void CompileTransform::perform(BuildContext&, Artifact&) {
		//TODO
	}

	void CompileTransform::wouldPerform(BuildContext&, Artifact&) {
		//TODO
	}

}}

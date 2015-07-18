#include "ExternalCompiler.hpp"

using std::string;

namespace redengine {
namespace build {

	// ======== ExternalCompilation ========

	ExternalCompiler::ExternalCompilation::ExternalCompilation(const string& executable,
			const string& source, CompileMode mode) : Compilation(source, mode) {
		command.addArgument(executable);
	}

	ExternalCompiler::ExternalCompilation::ExternalCompilation(const ExternalCompilation& compilation)
			: Compilation(compilation), command(compilation.command) {}

	// ======== ExternalCompiler ========

	ExternalCompiler::ExternalCompiler(const string& executable, Architecture architecture)
			: Compiler(architecture), executable(executable) {}

	ExternalCompiler::ExternalCompiler(const ExternalCompiler& compiler)
			: Compiler(compiler), executable(compiler.executable) {}

	void ExternalCompiler::setExecutable(const string& executable) {
		this->executable = executable;
	}

}}

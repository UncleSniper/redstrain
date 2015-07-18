#include "ExternalCompiler.hpp"

using std::string;
using redengine::redmond::Architecture;

namespace redengine {
namespace build {

	// ======== ExternalCompilation ========

	ExternalCompiler::ExternalCompilation::ExternalCompilation(const string& executable,
			const string& source, CompileMode mode) : Compilation(source, mode), ExternalInvocation(executable) {}

	ExternalCompiler::ExternalCompilation::ExternalCompilation(const ExternalCompilation& compilation)
			: Invocation(compilation), Compilation(compilation), ExternalInvocation(compilation) {}

	// ======== ExternalCompiler ========

	ExternalCompiler::ExternalCompiler(const string& executable, Architecture architecture)
			: ExternalTool(executable), Compiler(architecture) {}

	ExternalCompiler::ExternalCompiler(const ExternalCompiler& compiler)
			: ExternalTool(compiler), Compiler(compiler) {}

}}

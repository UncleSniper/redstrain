#include <redstrain/error/IllegalArgumentError.hpp>

#include "Compiler.hpp"

using std::string;
using redengine::redmond::Architecture;
using redengine::error::IllegalArgumentError;
using redengine::redmond::ARCH_I686;
using redengine::redmond::ARCH_X86_64;

namespace redengine {
namespace build {

	Compiler::Compiler(Architecture architecture) : architecture(architecture) {}

	Compiler::Compiler(const Compiler& compiler) : architecture(compiler.architecture) {}

	Compiler::~Compiler() {}

	Architecture Compiler::parseArchitecture(const string& specifier) {
		if(specifier == "i686" || specifier == "i486" || specifier == "i386")
			return ARCH_I686;
		if(specifier == "x86_64" || specifier == "x86_64_64" || specifier == "amd64" || specifier == "x64")
			return ARCH_X86_64;
		throw new IllegalArgumentError("Unrecognized architecture: " + specifier);
	}

}}

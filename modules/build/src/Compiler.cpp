#include "Compiler.hpp"

using redengine::redmond::Architecture;

namespace redengine {
namespace build {

	Compiler::Compiler(Architecture architecture) : architecture(architecture) {}

	Compiler::Compiler(const Compiler& compiler) : architecture(compiler.architecture) {}

	Compiler::~Compiler() {}

}}

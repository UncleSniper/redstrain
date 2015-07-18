#include "Linker.hpp"

using redengine::redmond::Architecture;

namespace redengine {
namespace build {

	Linker::Linker(Architecture architecture) : architecture(architecture) {}

	Linker::Linker(const Linker& linker) : architecture(linker.architecture) {}

	Linker::~Linker() {}

}}

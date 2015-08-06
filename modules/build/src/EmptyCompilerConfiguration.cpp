#include "EmptyCompilerConfiguration.hpp"

namespace redengine {
namespace build {

	EmptyCompilerConfiguration EmptyCompilerConfiguration::instance;

	EmptyCompilerConfiguration::EmptyCompilerConfiguration() {}

	EmptyCompilerConfiguration::EmptyCompilerConfiguration(const EmptyCompilerConfiguration& configuration)
			: CompilerConfiguration(configuration) {}

	void EmptyCompilerConfiguration::applyConfiguration(Compilation&) {}

}}

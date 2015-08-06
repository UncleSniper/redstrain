#include "EmptyLinkerConfiguration.hpp"

namespace redengine {
namespace build {

	EmptyLinkerConfiguration EmptyLinkerConfiguration::instance;

	EmptyLinkerConfiguration::EmptyLinkerConfiguration() {}

	EmptyLinkerConfiguration::EmptyLinkerConfiguration(const EmptyLinkerConfiguration& configuration)
			: LinkerConfiguration(configuration) {}

	void EmptyLinkerConfiguration::applyConfiguration(Linkage&) {}

}}

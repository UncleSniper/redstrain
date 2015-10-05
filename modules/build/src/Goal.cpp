#include "Goal.hpp"

using redengine::io::DefaultConfiguredOutputStream;

namespace redengine {
namespace build {

	Goal::Goal() {}

	Goal::Goal(const Goal& goal) : ReferenceCounted(goal), ComponentUIInfo(goal) {}

	Goal::~Goal() {}

	void Goal::dumpGoalAspects(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		dumpComponentUIInfoAspects(stream);
	}

}}

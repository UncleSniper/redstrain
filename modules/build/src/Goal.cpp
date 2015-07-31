#include "Goal.hpp"

namespace redengine {
namespace build {

	Goal::Goal() {}

	Goal::Goal(const Goal& goal) : ReferenceCounted(goal) {}

	Goal::~Goal() {}

}}

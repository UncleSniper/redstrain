#include "Action.hpp"

namespace redengine {
namespace build {

	Action::Action() {}

	Action::Action(const Action& action) : ReferenceCounted(action) {}

}}

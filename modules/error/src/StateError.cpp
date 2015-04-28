#include "StateError.hpp"

namespace redengine {
namespace error {

	StateError::StateError() {}

	StateError::StateError(const StateError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(StateError)

}}

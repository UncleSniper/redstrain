#include "CannotRelayerSubtreeError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	CannotRelayerSubtreeError::CannotRelayerSubtreeError() {}

	CannotRelayerSubtreeError::CannotRelayerSubtreeError(const CannotRelayerSubtreeError& error)
			: Error(error), DamnError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(CannotRelayerSubtreeError) {
		out << "Cannot relayer widget subtree, relayer the root instead";
	}

}}}

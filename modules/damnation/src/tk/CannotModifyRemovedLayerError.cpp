#include "CannotModifyRemovedLayerError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	CannotModifyRemovedLayerError::CannotModifyRemovedLayerError() {}

	CannotModifyRemovedLayerError::CannotModifyRemovedLayerError(const CannotModifyRemovedLayerError& error)
			: Error(error), DamnError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(CannotModifyRemovedLayerError) {
		out << "Cannot modify removed layer";
	}

}}}

#include "CannotModifyForeignLayerError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	CannotModifyForeignLayerError::CannotModifyForeignLayerError() {}

	CannotModifyForeignLayerError::CannotModifyForeignLayerError(const CannotModifyForeignLayerError& error)
			: Error(error), DamnError(error), IllegalArgumentError(error) {}

	REDSTRAIN_DEFINE_ERROR(CannotModifyForeignLayerError) {
		out << "Cannot modify layer foreign to screen";
	}

}}}

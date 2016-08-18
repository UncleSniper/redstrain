#include "ClipStackUnderflowError.hpp"

namespace redengine {
namespace damnation {

	ClipStackUnderflowError::ClipStackUnderflowError() {}

	ClipStackUnderflowError::ClipStackUnderflowError(const ClipStackUnderflowError& error)
			: Error(error), DamnError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(ClipStackUnderflowError) {
		out << "Terminal clip mask stack underflow";
	}

}}

#include "OriginStackUnderflowError.hpp"

namespace redengine {
namespace damnation {

	OriginStackUnderflowError::OriginStackUnderflowError() {}

	OriginStackUnderflowError::OriginStackUnderflowError(const OriginStackUnderflowError& error)
			: Error(error), DamnError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(OriginStackUnderflowError) {
		out << "Terminal origin stack underflow";
	}

}}

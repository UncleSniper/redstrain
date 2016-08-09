#include "ParameterizedStringStackUnderflowError.hpp"

namespace redengine {
namespace damnation {

	ParameterizedStringStackUnderflowError::ParameterizedStringStackUnderflowError() {}

	ParameterizedStringStackUnderflowError::ParameterizedStringStackUnderflowError(const
			ParameterizedStringStackUnderflowError& error) : Error(error), TParmError(error) {}

	REDSTRAIN_DEFINE_ERROR(ParameterizedStringStackUnderflowError) {
		out << "Parameterized string stack underflow";
	}

}}

#include "IllegalDeflateFunctionError.hpp"

namespace redengine {
namespace compress {

	IllegalDeflateFunctionError::IllegalDeflateFunctionError(int function) : function(function) {}

	IllegalDeflateFunctionError::IllegalDeflateFunctionError(const IllegalDeflateFunctionError& error)
			: Error(error), IllegalCompressionParameterError(error), function(error.function) {}

	REDSTRAIN_DEFINE_ERROR(IllegalDeflateFunctionError) {
		out << "Illegal compression function ID for deflate compression: " << function;
	}

}}

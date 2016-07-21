#include "CannotModifyCompressionParameterError.hpp"

namespace redengine {
namespace compress {

	CannotModifyCompressionParameterError::CannotModifyCompressionParameterError() {}

	CannotModifyCompressionParameterError::CannotModifyCompressionParameterError(const
			CannotModifyCompressionParameterError& error)
			: Error(error), CompressionError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(CannotModifyCompressionParameterError) {
		out << "Cannot modify compression parameter after compression/decompression has started";
	}

}}

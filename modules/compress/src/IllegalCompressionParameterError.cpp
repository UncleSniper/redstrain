#include "IllegalCompressionParameterError.hpp"

namespace redengine {
namespace compress {

	IllegalCompressionParameterError::IllegalCompressionParameterError() {}

	IllegalCompressionParameterError::IllegalCompressionParameterError(const IllegalCompressionParameterError& error)
			: Error(error), CompressionError(error), IllegalArgumentError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(IllegalCompressionParameterError)

}}

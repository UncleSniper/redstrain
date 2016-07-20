#include "CompressionError.hpp"

namespace redengine {
namespace compress {

	CompressionError::CompressionError() {}

	CompressionError::CompressionError(const CompressionError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(CompressionError)

}}
